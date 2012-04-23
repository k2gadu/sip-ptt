/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "vie_autotest.h"

#include "codec_primitives.h"
#include "common_types.h"
#include "general_primitives.h"
#include "tb_capture_device.h"
#include "tb_I420_codec.h"
#include "tb_interfaces.h"
#include "tb_video_channel.h"
#include "vie_autotest_defines.h"

class RenderFilter : public webrtc::ViEEffectFilter {
 public:
  RenderFilter()
      : last_render_width_(0),
        last_render_height_(0) {}

  ~RenderFilter() {}

  virtual int Transform(int size, unsigned char* frame_buffer,
                        unsigned int time_stamp, unsigned int width,
                        unsigned int height) {
    last_render_width_ = width;
    last_render_height_ = height;
    return 0;
  }
  unsigned int last_render_width_;
  unsigned int last_render_height_;
};

void ViEAutoTest::ViECodecStandardTest()
{
    TbInterfaces interfaces = TbInterfaces("ViECodecStandardTest");
    TbCaptureDevice capture_device = TbCaptureDevice(interfaces);

    int video_channel = -1;

    EXPECT_EQ(0, interfaces.base->CreateChannel(video_channel));
    EXPECT_EQ(0, interfaces.capture->ConnectCaptureDevice(
        capture_device.captureId, video_channel));

    ConfigureRtpRtcp(interfaces.rtp_rtcp,
                     video_channel);

    RenderInWindow(interfaces.render, capture_device.captureId, _window1, 0);
    RenderInWindow(interfaces.render, video_channel, _window2, 1);

    TestCodecs(interfaces, capture_device.captureId, video_channel,
               kDoNotForceResolution, kDoNotForceResolution);
}

void ViEAutoTest::ViECodecExtendedTest()
{
    {
        ViECodecAPITest();
        ViECodecStandardTest();
        ViECodecExternalCodecTest();

        TbInterfaces interfaces = TbInterfaces("ViECodecExtendedTest");
        webrtc::ViEBase* ptrViEBase = interfaces.base;
        webrtc::ViECapture* ptrViECapture = interfaces.capture;
        webrtc::ViERender* ptrViERender = interfaces.render;
        webrtc::ViECodec* ptrViECodec = interfaces.codec;
        webrtc::ViERTP_RTCP* ptrViERtpRtcp = interfaces.rtp_rtcp;
        webrtc::ViENetwork* ptrViENetwork = interfaces.network;

        TbCaptureDevice captureDevice = TbCaptureDevice(interfaces);
        int captureId = captureDevice.captureId;

        int videoChannel = -1;
        EXPECT_EQ(0, ptrViEBase->CreateChannel(videoChannel));
        EXPECT_EQ(0, ptrViECapture->ConnectCaptureDevice(
            captureId, videoChannel));

        EXPECT_EQ(0, ptrViERtpRtcp->SetRTCPStatus(
            videoChannel, webrtc::kRtcpCompound_RFC4585));
        EXPECT_EQ(0, ptrViERtpRtcp->SetKeyFrameRequestMethod(
            videoChannel, webrtc::kViEKeyFrameRequestPliRtcp));
        EXPECT_EQ(0, ptrViERtpRtcp->SetTMMBRStatus(videoChannel, true));
        EXPECT_EQ(0, ptrViERender->AddRenderer(
            captureId, _window1, 0, 0.0, 0.0, 1.0, 1.0));
        EXPECT_EQ(0, ptrViERender->AddRenderer(
            videoChannel, _window2, 1, 0.0, 0.0, 1.0, 1.0));
        EXPECT_EQ(0, ptrViERender->StartRender(captureId));
        EXPECT_EQ(0, ptrViERender->StartRender(videoChannel));

        webrtc::VideoCodec videoCodec;
        memset(&videoCodec, 0, sizeof(webrtc::VideoCodec));
        for (int idx = 0; idx < ptrViECodec->NumberOfCodecs(); idx++)
        {
            EXPECT_EQ(0, ptrViECodec->GetCodec(idx, videoCodec));

            if (videoCodec.codecType != webrtc::kVideoCodecI420)
            {
                videoCodec.width = 640;
                videoCodec.height = 480;
            }
            EXPECT_EQ(0, ptrViECodec->SetReceiveCodec(
                videoChannel, videoCodec));
        }

        const char* ipAddress = "127.0.0.1";
        const unsigned short rtpPort = 6000;
        EXPECT_EQ(0, ptrViENetwork->SetLocalReceiver(videoChannel, rtpPort));
        EXPECT_EQ(0, ptrViEBase->StartReceive(videoChannel));
        EXPECT_EQ(0, ptrViENetwork->SetSendDestination(
            videoChannel, ipAddress, rtpPort));
        EXPECT_EQ(0, ptrViEBase->StartSend(videoChannel));

        //
        // Codec specific tests
        //
        memset(&videoCodec, 0, sizeof(webrtc::VideoCodec));
        EXPECT_EQ(0, ptrViEBase->StopSend(videoChannel));
        ViEAutotestCodecObserver codecObserver;
        EXPECT_EQ(0, ptrViECodec->RegisterEncoderObserver(
            videoChannel, codecObserver));
        EXPECT_EQ(0, ptrViECodec->RegisterDecoderObserver(
            videoChannel, codecObserver));

        EXPECT_EQ(0, ptrViEBase->StopReceive(videoChannel));
        EXPECT_NE(0, ptrViEBase->StopSend(videoChannel));  // Already stopped

        EXPECT_EQ(0, ptrViERender->StopRender(videoChannel));
        EXPECT_EQ(0, ptrViERender->RemoveRenderer(captureId));
        EXPECT_EQ(0, ptrViERender->RemoveRenderer(videoChannel));
        EXPECT_EQ(0, ptrViECapture->DisconnectCaptureDevice(videoChannel));
        EXPECT_EQ(0, ptrViEBase->DeleteChannel(videoChannel));
    }

    //
    // Multiple send channels.
    //
    {
        // Create two channels, where the second channel is created from the
        // first channel. Send different resolutions on the channels and verify
        // the received streams.

        TbInterfaces ViE("ViECodecExtendedTest2");
        TbCaptureDevice tbCapture(ViE);

        // Create channel 1.
        int videoChannel1 = -1;
        EXPECT_EQ(0, ViE.base->CreateChannel(videoChannel1));

        // Create channel 2 based on the first channel.
        int videoChannel2 = -1;
        EXPECT_EQ(0, ViE.base->CreateChannel(videoChannel2, videoChannel1));
        EXPECT_NE(videoChannel1, videoChannel2) <<
            "Channel 2 should be unique.";

        unsigned short rtpPort1 = 12000;
        unsigned short rtpPort2 = 13000;
        EXPECT_EQ(0, ViE.network->SetLocalReceiver(
            videoChannel1, rtpPort1));
        EXPECT_EQ(0, ViE.network->SetSendDestination(
            videoChannel1, "127.0.0.1", rtpPort1));
        EXPECT_EQ(0, ViE.network->SetLocalReceiver(
            videoChannel2, rtpPort2));
        EXPECT_EQ(0, ViE.network->SetSendDestination(
            videoChannel2, "127.0.0.1", rtpPort2));

        tbCapture.ConnectTo(videoChannel1);
        tbCapture.ConnectTo(videoChannel2);

        EXPECT_EQ(0, ViE.rtp_rtcp->SetKeyFrameRequestMethod(
            videoChannel1, webrtc::kViEKeyFrameRequestPliRtcp));
        EXPECT_EQ(0, ViE.rtp_rtcp->SetKeyFrameRequestMethod(
            videoChannel2, webrtc::kViEKeyFrameRequestPliRtcp));
        EXPECT_EQ(0, ViE.render->AddRenderer(
            videoChannel1, _window1, 0, 0.0, 0.0, 1.0, 1.0));
        EXPECT_EQ(0, ViE.render->StartRender(videoChannel1));
        EXPECT_EQ(0, ViE.render->AddRenderer(
            videoChannel2, _window2, 0, 0.0, 0.0, 1.0, 1.0));
        EXPECT_EQ(0, ViE.render->StartRender(videoChannel2));

        // Set Send codec
        unsigned short codecWidth = 320;
        unsigned short codecHeight = 240;
        bool codecSet = false;
        webrtc::VideoCodec videoCodec;
        webrtc::VideoCodec sendCodec1;
        webrtc::VideoCodec sendCodec2;
        for (int idx = 0; idx < ViE.codec->NumberOfCodecs(); idx++)
        {
            EXPECT_EQ(0, ViE.codec->GetCodec(idx, videoCodec));
            EXPECT_EQ(0, ViE.codec->SetReceiveCodec(videoChannel1, videoCodec));
            if (videoCodec.codecType == webrtc::kVideoCodecVP8)
            {
                memcpy(&sendCodec1, &videoCodec, sizeof(videoCodec));
                sendCodec1.width = codecWidth;
                sendCodec1.height = codecHeight;
                EXPECT_EQ(0, ViE.codec->SetSendCodec(videoChannel1,
                                                     sendCodec1));
                memcpy(&sendCodec2, &videoCodec, sizeof(videoCodec));
                sendCodec2.width = 2 * codecWidth;
                sendCodec2.height = 2 * codecHeight;
                EXPECT_EQ(0, ViE.codec->SetSendCodec(videoChannel2,
                                                     sendCodec2));
                codecSet = true;
                break;
            }
        }
        EXPECT_TRUE(codecSet);


        // We need to verify using render effect filter since we won't trigger
        // a decode reset in loopback (due to using the same SSRC).
        RenderFilter filter1;
        RenderFilter filter2;
        EXPECT_EQ(0,
                  ViE.image_process->RegisterRenderEffectFilter(videoChannel1,
                                                                filter1));
        EXPECT_EQ(0,
                  ViE.image_process->RegisterRenderEffectFilter(videoChannel2,
                                                                filter2));

        EXPECT_EQ(0, ViE.base->StartReceive(videoChannel1));
        EXPECT_EQ(0, ViE.base->StartSend(videoChannel1));
        EXPECT_EQ(0, ViE.base->StartReceive(videoChannel2));
        EXPECT_EQ(0, ViE.base->StartSend(videoChannel2));


        AutoTestSleep(KAutoTestSleepTimeMs);

        EXPECT_EQ(0, ViE.base->StopReceive(videoChannel1));
        EXPECT_EQ(0, ViE.base->StopSend(videoChannel1));
        EXPECT_EQ(0, ViE.base->StopReceive(videoChannel2));
        EXPECT_EQ(0, ViE.base->StopSend(videoChannel2));

        EXPECT_EQ(0, ViE.image_process->DeregisterRenderEffectFilter(
            videoChannel1));
        EXPECT_EQ(0, ViE.image_process->DeregisterRenderEffectFilter(
            videoChannel2));
        EXPECT_EQ(sendCodec1.width, filter1.last_render_width_);
        EXPECT_EQ(sendCodec1.height, filter1.last_render_height_);
        EXPECT_EQ(sendCodec2.width, filter2.last_render_width_);
        EXPECT_EQ(sendCodec2.height, filter2.last_render_height_);

        EXPECT_EQ(0, ViE.base->DeleteChannel(videoChannel1));
        EXPECT_EQ(0, ViE.base->DeleteChannel(videoChannel2));
    }
}

void ViEAutoTest::ViECodecAPITest()
{
    // ***************************************************************
    // Begin create/initialize WebRTC Video Engine for testing
    // ***************************************************************
    webrtc::VideoEngine* ptrViE = NULL;
    ptrViE = webrtc::VideoEngine::Create();
    EXPECT_TRUE(ptrViE != NULL);

    webrtc::ViEBase* ptrViEBase = webrtc::ViEBase::GetInterface(ptrViE);
    EXPECT_TRUE(ptrViEBase != NULL);

    EXPECT_EQ(0, ptrViEBase->Init());

    int videoChannel = -1;
    EXPECT_EQ(0, ptrViEBase->CreateChannel(videoChannel));

    webrtc::ViECodec* ptrViECodec = webrtc::ViECodec::GetInterface(ptrViE);
    EXPECT_TRUE(ptrViECodec != NULL);

    //***************************************************************
    //	Engine ready. Begin testing class
    //***************************************************************

    //
    // SendCodec
    //
    webrtc::VideoCodec videoCodec;
    memset(&videoCodec, 0, sizeof(webrtc::VideoCodec));

    const int numberOfCodecs = ptrViECodec->NumberOfCodecs();
    EXPECT_GT(numberOfCodecs, 0);

    SetSendCodec(webrtc::kVideoCodecVP8, ptrViECodec, videoChannel,
                 kDoNotForceResolution, kDoNotForceResolution);

    memset(&videoCodec, 0, sizeof(videoCodec));
    EXPECT_EQ(0, ptrViECodec->GetSendCodec(videoChannel, videoCodec));
    EXPECT_EQ(webrtc::kVideoCodecVP8, videoCodec.codecType);
    // Verify that the target bit rate is equal to the start bitrate.
    unsigned int target_bitrate = 0;
    EXPECT_EQ(0, ptrViECodec->GetCodecTargetBitrate(videoChannel,
                                                    &target_bitrate));
    EXPECT_EQ(videoCodec.startBitrate, target_bitrate);

    SetSendCodec(webrtc::kVideoCodecI420, ptrViECodec, videoChannel,
                 kDoNotForceResolution, kDoNotForceResolution);
    memset(&videoCodec, 0, sizeof(videoCodec));
    EXPECT_EQ(0, ptrViECodec->GetSendCodec(videoChannel, videoCodec));
    EXPECT_EQ(webrtc::kVideoCodecI420, videoCodec.codecType);

    //***************************************************************
    //	Testing finished. Tear down Video Engine
    //***************************************************************

    EXPECT_EQ(0, ptrViEBase->DeleteChannel(videoChannel));

    EXPECT_EQ(0, ptrViECodec->Release());
    EXPECT_EQ(0, ptrViEBase->Release());
    EXPECT_TRUE(webrtc::VideoEngine::Delete(ptrViE));
}

#ifdef WEBRTC_VIDEO_ENGINE_EXTERNAL_CODEC_API
#include "vie_external_codec.h"
#endif
void ViEAutoTest::ViECodecExternalCodecTest()
{
    // ***************************************************************
    // Begin create/initialize WebRTC Video Engine for testing
    // ***************************************************************


    // ***************************************************************
    // Engine ready. Begin testing class
    // ***************************************************************

#ifdef WEBRTC_VIDEO_ENGINE_EXTERNAL_CODEC_API
    {
        TbInterfaces ViE("ViEExternalCodec");
        TbCaptureDevice captureDevice(ViE);
        TbVideoChannel channel(
            ViE, webrtc::kVideoCodecI420, 352,288,30,(352*288*3*8*30)/(2*1000));

        captureDevice.ConnectTo(channel.videoChannel);

        EXPECT_EQ(0, ViE.render->AddRenderer(
            channel.videoChannel, _window1, 0, 0.0, 0.0, 1.0, 1.0));
        EXPECT_EQ(0, ViE.render->StartRender(channel.videoChannel));

        channel.StartReceive();
        channel.StartSend();

        ViETest::Log("Using internal I420 codec");
        AutoTestSleep(KAutoTestSleepTimeMs/2);

        webrtc::ViEExternalCodec* ptrViEExtCodec =
            webrtc::ViEExternalCodec::GetInterface(ViE.video_engine);
        EXPECT_TRUE(ptrViEExtCodec != NULL);

        webrtc::VideoCodec codecStruct;

        EXPECT_EQ(0, ViE.codec->GetSendCodec(
            channel.videoChannel, codecStruct));

        // Use external encoder instead
        {
            TbI420Encoder extEncoder;

            // Test to register on wrong channel
            EXPECT_NE(0, ptrViEExtCodec->RegisterExternalSendCodec(
                channel.videoChannel+5,codecStruct.plType,&extEncoder));
            EXPECT_EQ(kViECodecInvalidArgument, ViE.LastError());

            EXPECT_EQ(0, ptrViEExtCodec->RegisterExternalSendCodec(
                channel.videoChannel,codecStruct.plType,&extEncoder));

            // Use new external encoder
            EXPECT_EQ(0, ViE.codec->SetSendCodec(
                channel.videoChannel, codecStruct));

            TbI420Decoder extDecoder;
            EXPECT_EQ(0, ptrViEExtCodec->RegisterExternalReceiveCodec(
                channel.videoChannel,codecStruct.plType,&extDecoder));

            EXPECT_EQ(0, ViE.codec->SetReceiveCodec(
                channel.videoChannel, codecStruct));

            ViETest::Log("Using external I420 codec");
            AutoTestSleep(KAutoTestSleepTimeMs);

            // Test to deregister on wrong channel
            EXPECT_NE(0, ptrViEExtCodec->DeRegisterExternalSendCodec(
                channel.videoChannel+5,codecStruct.plType));
            EXPECT_EQ(kViECodecInvalidArgument, ViE.LastError());

            // Test to deregister wrong payload type.
            EXPECT_NE(0, ptrViEExtCodec->DeRegisterExternalSendCodec(
                channel.videoChannel,codecStruct.plType-1));

            // Deregister external send codec
            EXPECT_EQ(0, ptrViEExtCodec->DeRegisterExternalSendCodec(
                channel.videoChannel,codecStruct.plType));

            EXPECT_EQ(0, ptrViEExtCodec->DeRegisterExternalReceiveCodec(
                channel.videoChannel,codecStruct.plType));

            // Verify that the encoder and decoder has been used
            TbI420Encoder::FunctionCalls encodeCalls =
                extEncoder.GetFunctionCalls();
            EXPECT_EQ(1, encodeCalls.InitEncode);
            EXPECT_EQ(1, encodeCalls.Release);
            EXPECT_EQ(1, encodeCalls.RegisterEncodeCompleteCallback);
            EXPECT_GT(encodeCalls.Encode, 30);
            EXPECT_GT(encodeCalls.SetRates, 1);
            EXPECT_GT(encodeCalls.SetPacketLoss, 1);

            TbI420Decoder::FunctionCalls decodeCalls =
                extDecoder.GetFunctionCalls();
            EXPECT_EQ(1, decodeCalls.InitDecode);
            EXPECT_EQ(1, decodeCalls.Release);
            EXPECT_EQ(1, decodeCalls.RegisterDecodeCompleteCallback);
            EXPECT_GT(decodeCalls.Decode, 30);

            ViETest::Log("Changing payload type Using external I420 codec");

            codecStruct.plType = codecStruct.plType - 1;
            EXPECT_EQ(0, ptrViEExtCodec->RegisterExternalReceiveCodec(
                channel.videoChannel, codecStruct.plType, &extDecoder));

            EXPECT_EQ(0, ViE.codec->SetReceiveCodec(
                channel.videoChannel, codecStruct));

            EXPECT_EQ(0, ptrViEExtCodec->RegisterExternalSendCodec(
                channel.videoChannel, codecStruct.plType, &extEncoder));

            // Use new external encoder
            EXPECT_EQ(0, ViE.codec->SetSendCodec(
                channel.videoChannel, codecStruct));

            AutoTestSleep(KAutoTestSleepTimeMs/2);

            //***************************************************************
            //	Testing finished. Tear down Video Engine
            //***************************************************************

            EXPECT_EQ(0, ptrViEExtCodec->DeRegisterExternalSendCodec(
                channel.videoChannel,codecStruct.plType));
            EXPECT_EQ(0, ptrViEExtCodec->DeRegisterExternalReceiveCodec(
                channel.videoChannel,codecStruct.plType));

            // Verify that the encoder and decoder has been used
            encodeCalls = extEncoder.GetFunctionCalls();
            EXPECT_EQ(2, encodeCalls.InitEncode);
            EXPECT_EQ(2, encodeCalls.Release);
            EXPECT_EQ(2, encodeCalls.RegisterEncodeCompleteCallback);
            EXPECT_GT(encodeCalls.Encode, 30);
            EXPECT_GT(encodeCalls.SetRates, 1);
            EXPECT_GT(encodeCalls.SetPacketLoss, 1);

            decodeCalls = extDecoder.GetFunctionCalls();

            EXPECT_EQ(2, decodeCalls.InitDecode);
            EXPECT_EQ(2, decodeCalls.Release);
            EXPECT_EQ(2, decodeCalls.RegisterDecodeCompleteCallback);
            EXPECT_GT(decodeCalls.Decode, 30);

            EXPECT_EQ(0, ptrViEExtCodec->Release());
        }  // tbI420Encoder and extDecoder goes out of scope

        ViETest::Log("Using internal I420 codec");
        AutoTestSleep(KAutoTestSleepTimeMs/2);

    }

#else
    ViETest::Log(" ViEExternalCodec not enabled\n");
#endif
}
