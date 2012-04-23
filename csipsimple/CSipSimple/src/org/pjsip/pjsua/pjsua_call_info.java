/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.pjsip.pjsua;

public class pjsua_call_info {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public pjsua_call_info(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(pjsua_call_info obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        pjsuaJNI.delete_pjsua_call_info(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setId(int value) {
    pjsuaJNI.pjsua_call_info_id_set(swigCPtr, this, value);
  }

  public int getId() {
    return pjsuaJNI.pjsua_call_info_id_get(swigCPtr, this);
  }

  public void setRole(SWIGTYPE_p_pjsip_role_e value) {
    pjsuaJNI.pjsua_call_info_role_set(swigCPtr, this, SWIGTYPE_p_pjsip_role_e.getCPtr(value));
  }

  public SWIGTYPE_p_pjsip_role_e getRole() {
    return new SWIGTYPE_p_pjsip_role_e(pjsuaJNI.pjsua_call_info_role_get(swigCPtr, this), true);
  }

  public void setAcc_id(int value) {
    pjsuaJNI.pjsua_call_info_acc_id_set(swigCPtr, this, value);
  }

  public int getAcc_id() {
    return pjsuaJNI.pjsua_call_info_acc_id_get(swigCPtr, this);
  }

  public void setLocal_info(pj_str_t value) {
    pjsuaJNI.pjsua_call_info_local_info_set(swigCPtr, this, pj_str_t.getCPtr(value), value);
  }

  public pj_str_t getLocal_info() {
    long cPtr = pjsuaJNI.pjsua_call_info_local_info_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_str_t(cPtr, false);
  }

  public void setLocal_contact(pj_str_t value) {
    pjsuaJNI.pjsua_call_info_local_contact_set(swigCPtr, this, pj_str_t.getCPtr(value), value);
  }

  public pj_str_t getLocal_contact() {
    long cPtr = pjsuaJNI.pjsua_call_info_local_contact_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_str_t(cPtr, false);
  }

  public void setRemote_info(pj_str_t value) {
    pjsuaJNI.pjsua_call_info_remote_info_set(swigCPtr, this, pj_str_t.getCPtr(value), value);
  }

  public pj_str_t getRemote_info() {
    long cPtr = pjsuaJNI.pjsua_call_info_remote_info_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_str_t(cPtr, false);
  }

  public void setRemote_contact(pj_str_t value) {
    pjsuaJNI.pjsua_call_info_remote_contact_set(swigCPtr, this, pj_str_t.getCPtr(value), value);
  }

  public pj_str_t getRemote_contact() {
    long cPtr = pjsuaJNI.pjsua_call_info_remote_contact_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_str_t(cPtr, false);
  }

  public void setCall_id(pj_str_t value) {
    pjsuaJNI.pjsua_call_info_call_id_set(swigCPtr, this, pj_str_t.getCPtr(value), value);
  }

  public pj_str_t getCall_id() {
    long cPtr = pjsuaJNI.pjsua_call_info_call_id_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_str_t(cPtr, false);
  }

  public void setSetting(pjsua_call_setting value) {
    pjsuaJNI.pjsua_call_info_setting_set(swigCPtr, this, pjsua_call_setting.getCPtr(value), value);
  }

  public pjsua_call_setting getSetting() {
    long cPtr = pjsuaJNI.pjsua_call_info_setting_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pjsua_call_setting(cPtr, false);
  }

  public void setState(pjsip_inv_state value) {
    pjsuaJNI.pjsua_call_info_state_set(swigCPtr, this, value.swigValue());
  }

  public pjsip_inv_state getState() {
    return pjsip_inv_state.swigToEnum(pjsuaJNI.pjsua_call_info_state_get(swigCPtr, this));
  }

  public void setState_text(pj_str_t value) {
    pjsuaJNI.pjsua_call_info_state_text_set(swigCPtr, this, pj_str_t.getCPtr(value), value);
  }

  public pj_str_t getState_text() {
    long cPtr = pjsuaJNI.pjsua_call_info_state_text_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_str_t(cPtr, false);
  }

  public void setLast_status(pjsip_status_code value) {
    pjsuaJNI.pjsua_call_info_last_status_set(swigCPtr, this, value.swigValue());
  }

  public pjsip_status_code getLast_status() {
    return pjsip_status_code.swigToEnum(pjsuaJNI.pjsua_call_info_last_status_get(swigCPtr, this));
  }

  public void setLast_status_text(pj_str_t value) {
    pjsuaJNI.pjsua_call_info_last_status_text_set(swigCPtr, this, pj_str_t.getCPtr(value), value);
  }

  public pj_str_t getLast_status_text() {
    long cPtr = pjsuaJNI.pjsua_call_info_last_status_text_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_str_t(cPtr, false);
  }

  public void setMedia_status(pjsua_call_media_status value) {
    pjsuaJNI.pjsua_call_info_media_status_set(swigCPtr, this, value.swigValue());
  }

  public pjsua_call_media_status getMedia_status() {
    return pjsua_call_media_status.swigToEnum(pjsuaJNI.pjsua_call_info_media_status_get(swigCPtr, this));
  }

  public void setMedia_dir(pjmedia_dir value) {
    pjsuaJNI.pjsua_call_info_media_dir_set(swigCPtr, this, value.swigValue());
  }

  public pjmedia_dir getMedia_dir() {
    return pjmedia_dir.swigToEnum(pjsuaJNI.pjsua_call_info_media_dir_get(swigCPtr, this));
  }

  public void setConf_slot(int value) {
    pjsuaJNI.pjsua_call_info_conf_slot_set(swigCPtr, this, value);
  }

  public int getConf_slot() {
    return pjsuaJNI.pjsua_call_info_conf_slot_get(swigCPtr, this);
  }

  public void setMedia_cnt(long value) {
    pjsuaJNI.pjsua_call_info_media_cnt_set(swigCPtr, this, value);
  }

  public long getMedia_cnt() {
    return pjsuaJNI.pjsua_call_info_media_cnt_get(swigCPtr, this);
  }

  public void setConnect_duration(pj_time_val value) {
    pjsuaJNI.pjsua_call_info_connect_duration_set(swigCPtr, this, pj_time_val.getCPtr(value), value);
  }

  public pj_time_val getConnect_duration() {
    long cPtr = pjsuaJNI.pjsua_call_info_connect_duration_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_time_val(cPtr, false);
  }

  public void setTotal_duration(pj_time_val value) {
    pjsuaJNI.pjsua_call_info_total_duration_set(swigCPtr, this, pj_time_val.getCPtr(value), value);
  }

  public pj_time_val getTotal_duration() {
    long cPtr = pjsuaJNI.pjsua_call_info_total_duration_get(swigCPtr, this);
    return (cPtr == 0) ? null : new pj_time_val(cPtr, false);
  }

  public void setRem_offerer(int value) {
    pjsuaJNI.pjsua_call_info_rem_offerer_set(swigCPtr, this, value);
  }

  public int getRem_offerer() {
    return pjsuaJNI.pjsua_call_info_rem_offerer_get(swigCPtr, this);
  }

  public void setRem_aud_cnt(long value) {
    pjsuaJNI.pjsua_call_info_rem_aud_cnt_set(swigCPtr, this, value);
  }

  public long getRem_aud_cnt() {
    return pjsuaJNI.pjsua_call_info_rem_aud_cnt_get(swigCPtr, this);
  }

  public void setRem_vid_cnt(long value) {
    pjsuaJNI.pjsua_call_info_rem_vid_cnt_set(swigCPtr, this, value);
  }

  public long getRem_vid_cnt() {
    return pjsuaJNI.pjsua_call_info_rem_vid_cnt_get(swigCPtr, this);
  }

  public pjsua_call_info() {
    this(pjsuaJNI.new_pjsua_call_info(), true);
  }

}
