#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= opus
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MY_MOAI_ROOT)/src/zlcore/zl_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/bands.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/celt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/celt_lpc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/cwrs.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/entcode.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/entdec.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/entenc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/kiss_fft.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/laplace.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/mathops.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/mdct.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/modes.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/opus_custom_demo.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/pitch.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/quant_bands.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/rate.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/vq.c
	LOCAL_C_INCLUDES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/celt/
	LOCAL_C_INCLUDES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/A2NLSF.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/CNG.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/HP_variable_cutoff.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/LPC_analysis_filter.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/LPC_inv_pred_gain.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/LP_variable_cutoff.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF2A.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF_VQ.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF_VQ_weights_laroia.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF_decode.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF_del_dec_quant.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF_encode.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF_stabilize.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NLSF_unpack.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NSQ.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/NSQ_del_dec.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/PLC.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/VAD.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/VQ_WMat_EC.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/ana_filt_bank_1.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/biquad_alt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/bwexpander.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/bwexpander_32.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/check_control_input.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/code_signs.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/control_SNR.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/control_audio_bandwidth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/control_codec.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/debug.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/dec_API.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/decode_core.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/decode_frame.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/decode_indices.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/decode_parameters.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/decode_pitch.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/decode_pulses.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/decoder_set_fs.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/enc_API.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/encode_indices.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/encode_pulses.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/gain_quant.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/init_decoder.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/init_encoder.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/inner_prod_aligned.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/interpolate.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/lin2log.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/log2lin.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/pitch_est_tables.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/process_NLSFs.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/quant_LTP_gains.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler_down2.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler_down2_3.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler_private_AR2.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler_private_IIR_FIR.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler_private_down_FIR.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler_private_up2_HQ.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/resampler_rom.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/shell_coder.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/sigm_Q15.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/sort.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/stereo_LR_to_MS.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/stereo_MS_to_LR.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/stereo_decode_pred.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/stereo_encode_pred.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/stereo_find_predictor.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/stereo_quant_pred.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/sum_sqr_shift.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/table_LSF_cos.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/tables_LTP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/tables_NLSF_CB_NB_MB.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/tables_NLSF_CB_WB.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/tables_gain.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/tables_other.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/tables_pitch_lag.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/tables_pulses_per_block.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/LPC_analysis_filter_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/LPC_inv_pred_gain_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/LTP_analysis_filter_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/LTP_scale_ctrl_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/apply_sine_window_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/autocorrelation_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/burg_modified_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/bwexpander_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/corrMatrix_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/encode_frame_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/energy_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/find_LPC_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/find_LTP_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/find_pitch_lags_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/find_pred_coefs_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/inner_product_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/k2a_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/levinsondurbin_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/noise_shape_analysis_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/pitch_analysis_core_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/prefilter_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/process_gains_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/regularize_correlations_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/residual_energy_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/scale_copy_vector_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/scale_vector_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/schur_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/solve_LS_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/sort_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/warped_autocorrelation_FLP.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/float/wrappers_FLP.c
	LOCAL_C_INCLUDES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/silk/
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/src/opus_decoder.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/src/opus_multistream.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/src/opus_encoder.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/src/repacketizer.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opus-1.0.2/src/opus.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opusfile/src/http.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opusfile/src/info.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opusfile/src/internal.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opusfile/src/opusfile.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opusfile/src/stream.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/opusfile/src/wincerts.c

	include $(BUILD_STATIC_LIBRARY)