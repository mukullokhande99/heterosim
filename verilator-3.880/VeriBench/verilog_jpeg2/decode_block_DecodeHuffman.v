// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2015.4
// Copyright (C) 2015 Xilinx Inc. All rights reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module decode_block_DecodeHuffman (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
        ap_idle,
        ap_ready,
        Xhuff_huffval_address0,
        Xhuff_huffval_ce0,
        Xhuff_huffval_q0,
        tmp_s,
        Dhuff_maxcode_address0,
        Dhuff_maxcode_ce0,
        Dhuff_maxcode_q0,
        tmp_1,
        Dhuff_mincode_address0,
        Dhuff_mincode_ce0,
        Dhuff_mincode_q0,
        tmp_2,
        Dhuff_valptr_address0,
        Dhuff_valptr_ce0,
        Dhuff_valptr_q0,
        tmp_3,
        CurHuffReadBuf_address0,
        CurHuffReadBuf_ce0,
        CurHuffReadBuf_q0,
        read_position_i,
        read_position_o,
        read_position_o_ap_vld,
        off_i,
        off_o,
        off_o_ap_vld,
        current_read_byte_i,
        current_read_byte_o,
        current_read_byte_o_ap_vld,
        ap_return
);

parameter    ap_const_logic_1 = 1'b1;
parameter    ap_const_logic_0 = 1'b0;
parameter    ap_ST_st1_fsm_0 = 15'b1;
parameter    ap_ST_st2_fsm_1 = 15'b10;
parameter    ap_ST_st3_fsm_2 = 15'b100;
parameter    ap_ST_st4_fsm_3 = 15'b1000;
parameter    ap_ST_st5_fsm_4 = 15'b10000;
parameter    ap_ST_st6_fsm_5 = 15'b100000;
parameter    ap_ST_st7_fsm_6 = 15'b1000000;
parameter    ap_ST_st8_fsm_7 = 15'b10000000;
parameter    ap_ST_st9_fsm_8 = 15'b100000000;
parameter    ap_ST_st10_fsm_9 = 15'b1000000000;
parameter    ap_ST_st11_fsm_10 = 15'b10000000000;
parameter    ap_ST_st12_fsm_11 = 15'b100000000000;
parameter    ap_ST_st13_fsm_12 = 15'b1000000000000;
parameter    ap_ST_st14_fsm_13 = 15'b10000000000000;
parameter    ap_ST_st15_fsm_14 = 15'b100000000000000;
parameter    ap_const_lv32_0 = 32'b00000000000000000000000000000000;
parameter    ap_const_lv1_1 = 1'b1;
parameter    ap_const_lv32_4 = 32'b100;
parameter    ap_const_lv32_5 = 32'b101;
parameter    ap_const_lv32_6 = 32'b110;
parameter    ap_const_lv1_0 = 1'b0;
parameter    ap_const_lv32_7 = 32'b111;
parameter    ap_const_lv32_B = 32'b1011;
parameter    ap_const_lv32_C = 32'b1100;
parameter    ap_const_lv32_D = 32'b1101;
parameter    ap_const_lv32_1 = 32'b1;
parameter    ap_const_lv32_2 = 32'b10;
parameter    ap_const_lv32_3 = 32'b11;
parameter    ap_const_lv32_8 = 32'b1000;
parameter    ap_const_lv32_9 = 32'b1001;
parameter    ap_const_lv32_A = 32'b1010;
parameter    ap_const_lv8_5 = 8'b101;
parameter    ap_const_lv8_2 = 8'b10;
parameter    ap_const_lv8_0 = 8'b00000000;
parameter    ap_const_lv32_E = 32'b1110;
parameter    ap_true = 1'b1;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
output  [9:0] Xhuff_huffval_address0;
output   Xhuff_huffval_ce0;
input  [31:0] Xhuff_huffval_q0;
input  [7:0] tmp_s;
output  [6:0] Dhuff_maxcode_address0;
output   Dhuff_maxcode_ce0;
input  [31:0] Dhuff_maxcode_q0;
input  [7:0] tmp_1;
output  [6:0] Dhuff_mincode_address0;
output   Dhuff_mincode_ce0;
input  [31:0] Dhuff_mincode_q0;
input  [7:0] tmp_2;
output  [6:0] Dhuff_valptr_address0;
output   Dhuff_valptr_ce0;
input  [31:0] Dhuff_valptr_q0;
input  [7:0] tmp_3;
output  [5:0] CurHuffReadBuf_address0;
output   CurHuffReadBuf_ce0;
input  [7:0] CurHuffReadBuf_q0;
input  [31:0] read_position_i;
output  [31:0] read_position_o;
output   read_position_o_ap_vld;
input  [31:0] off_i;
output  [31:0] off_o;
output   off_o_ap_vld;
input  [31:0] current_read_byte_i;
output  [31:0] current_read_byte_o;
output   current_read_byte_o_ap_vld;
output  [31:0] ap_return;

reg ap_done;
reg ap_idle;
reg ap_ready;
reg Xhuff_huffval_ce0;
reg Dhuff_maxcode_ce0;
reg Dhuff_mincode_ce0;
reg Dhuff_valptr_ce0;
reg CurHuffReadBuf_ce0;
reg[31:0] read_position_o;
reg[31:0] off_o;
reg[31:0] current_read_byte_o;
reg[31:0] ap_return;
(* fsm_encoding = "none" *) reg   [14:0] ap_CS_fsm = 15'b1;
reg    ap_sig_cseq_ST_st1_fsm_0;
reg    ap_sig_bdd_33;
wire   [7:0] tmp_66_fu_176_p2;
reg   [7:0] tmp_66_reg_324;
reg    ap_sig_cseq_ST_st5_fsm_4;
reg    ap_sig_bdd_89;
wire   [7:0] tmp_67_fu_194_p2;
reg   [7:0] tmp_67_reg_329;
wire   [7:0] tmp_68_fu_212_p2;
reg   [7:0] tmp_68_reg_334;
wire   [10:0] tmp_69_fu_234_p2;
reg   [10:0] tmp_69_reg_339;
wire   [31:0] code1_trunc_ext_fu_240_p1;
wire   [7:0] tmp_30_fu_244_p1;
reg   [7:0] tmp_30_reg_349;
reg    ap_sig_cseq_ST_st6_fsm_5;
reg    ap_sig_bdd_106;
wire   [31:0] l_1_fu_264_p2;
reg   [31:0] l_1_reg_363;
reg    ap_sig_cseq_ST_st7_fsm_6;
reg    ap_sig_bdd_117;
wire   [0:0] tmp_4_fu_258_p2;
wire   [30:0] tmp_31_fu_288_p1;
reg   [30:0] tmp_31_reg_378;
reg    ap_sig_cseq_ST_st8_fsm_7;
reg    ap_sig_bdd_136;
wire   [31:0] code_1_fu_292_p3;
reg    ap_sig_cseq_ST_st12_fsm_11;
reg    ap_sig_bdd_145;
wire   [10:0] tmp_73_fu_315_p2;
reg   [10:0] tmp_73_reg_388;
reg    ap_sig_cseq_ST_st13_fsm_12;
reg    ap_sig_bdd_154;
reg    ap_sig_cseq_ST_st14_fsm_13;
reg    ap_sig_bdd_163;
reg    grp_decode_block_buf_getb_fu_152_ap_start;
wire    grp_decode_block_buf_getb_fu_152_ap_done;
wire    grp_decode_block_buf_getb_fu_152_ap_idle;
wire    grp_decode_block_buf_getb_fu_152_ap_ready;
wire    grp_decode_block_buf_getb_fu_152_ap_ce;
wire   [5:0] grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_address0;
wire    grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_ce0;
wire   [7:0] grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_q0;
wire   [31:0] grp_decode_block_buf_getb_fu_152_read_position_i;
wire   [31:0] grp_decode_block_buf_getb_fu_152_read_position_o;
wire    grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld;
wire   [31:0] grp_decode_block_buf_getb_fu_152_off_i;
wire   [31:0] grp_decode_block_buf_getb_fu_152_off_o;
wire    grp_decode_block_buf_getb_fu_152_off_o_ap_vld;
wire   [31:0] grp_decode_block_buf_getb_fu_152_current_read_byte_i;
wire   [31:0] grp_decode_block_buf_getb_fu_152_current_read_byte_o;
wire    grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld;
wire   [0:0] grp_decode_block_buf_getb_fu_152_ap_return;
reg   [31:0] l_reg_130;
reg   [31:0] code1_reg_142;
reg    grp_decode_block_buf_getb_fu_152_ap_start_ap_start_reg = 1'b0;
reg   [14:0] ap_NS_fsm;
reg    ap_sig_nseq_ST_st8_fsm_7;
reg    ap_sig_bdd_206;
reg    ap_sig_cseq_ST_st2_fsm_1;
reg    ap_sig_bdd_213;
reg    ap_sig_cseq_ST_st3_fsm_2;
reg    ap_sig_bdd_222;
reg    ap_sig_cseq_ST_st4_fsm_3;
reg    ap_sig_bdd_231;
reg    ap_sig_cseq_ST_st9_fsm_8;
reg    ap_sig_bdd_244;
reg    ap_sig_cseq_ST_st10_fsm_9;
reg    ap_sig_bdd_253;
reg    ap_sig_cseq_ST_st11_fsm_10;
reg    ap_sig_bdd_262;
wire  signed [63:0] tmp_78_cast_fu_253_p1;
wire  signed [63:0] tmp_79_cast_fu_274_p1;
wire  signed [63:0] tmp_80_cast_fu_283_p1;
wire  signed [63:0] tmp_81_cast_fu_320_p1;
wire   [7:0] tmp_fu_164_p2;
wire   [7:0] tmp_23_fu_170_p2;
wire   [7:0] tmp_25_fu_182_p2;
wire   [7:0] tmp_26_fu_188_p2;
wire   [7:0] tmp_27_fu_200_p2;
wire   [7:0] tmp_28_fu_206_p2;
wire   [2:0] tmp_29_fu_222_p1;
wire   [10:0] p_shl_cast_fu_226_p3;
wire  signed [10:0] tmp_cast_cast_fu_218_p1;
wire   [7:0] tmp_70_fu_248_p2;
wire   [7:0] tmp_71_fu_270_p2;
wire   [7:0] tmp_72_fu_279_p2;
wire   [31:0] tmp_24_fu_299_p2;
wire   [31:0] p_fu_305_p2;
wire   [10:0] tmp_32_fu_311_p1;
reg   [31:0] ap_return_preg = 32'b00000000000000000000000000000000;
reg    ap_sig_cseq_ST_st15_fsm_14;
reg    ap_sig_bdd_441;


decode_block_buf_getb grp_decode_block_buf_getb_fu_152(
    .ap_clk( ap_clk ),
    .ap_rst( ap_rst ),
    .ap_start( grp_decode_block_buf_getb_fu_152_ap_start ),
    .ap_done( grp_decode_block_buf_getb_fu_152_ap_done ),
    .ap_idle( grp_decode_block_buf_getb_fu_152_ap_idle ),
    .ap_ready( grp_decode_block_buf_getb_fu_152_ap_ready ),
    .ap_ce( grp_decode_block_buf_getb_fu_152_ap_ce ),
    .CurHuffReadBuf_address0( grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_address0 ),
    .CurHuffReadBuf_ce0( grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_ce0 ),
    .CurHuffReadBuf_q0( grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_q0 ),
    .read_position_i( grp_decode_block_buf_getb_fu_152_read_position_i ),
    .read_position_o( grp_decode_block_buf_getb_fu_152_read_position_o ),
    .read_position_o_ap_vld( grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld ),
    .off_i( grp_decode_block_buf_getb_fu_152_off_i ),
    .off_o( grp_decode_block_buf_getb_fu_152_off_o ),
    .off_o_ap_vld( grp_decode_block_buf_getb_fu_152_off_o_ap_vld ),
    .current_read_byte_i( grp_decode_block_buf_getb_fu_152_current_read_byte_i ),
    .current_read_byte_o( grp_decode_block_buf_getb_fu_152_current_read_byte_o ),
    .current_read_byte_o_ap_vld( grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld ),
    .ap_return( grp_decode_block_buf_getb_fu_152_ap_return )
);



always @ (posedge ap_clk) begin : ap_ret_ap_CS_fsm
    if (ap_rst == 1'b1) begin
        ap_CS_fsm <= ap_ST_st1_fsm_0;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

always @ (posedge ap_clk) begin : ap_ret_ap_return_preg
    if (ap_rst == 1'b1) begin
        ap_return_preg <= ap_const_lv32_0;
    end else begin
        if ((ap_const_logic_1 == ap_sig_cseq_ST_st15_fsm_14)) begin
            ap_return_preg <= Xhuff_huffval_q0;
        end
    end
end

always @ (posedge ap_clk) begin : ap_ret_grp_decode_block_buf_getb_fu_152_ap_start_ap_start_reg
    if (ap_rst == 1'b1) begin
        grp_decode_block_buf_getb_fu_152_ap_start_ap_start_reg <= ap_const_logic_0;
    end else begin
        if (((ap_const_logic_1 == ap_sig_cseq_ST_st7_fsm_6) & (ap_const_logic_1 == ap_sig_nseq_ST_st8_fsm_7))) begin
            grp_decode_block_buf_getb_fu_152_ap_start_ap_start_reg <= ap_const_logic_1;
        end else if ((ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_ap_ready)) begin
            grp_decode_block_buf_getb_fu_152_ap_start_ap_start_reg <= ap_const_logic_0;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st12_fsm_11)) begin
        code1_reg_142 <= code_1_fu_292_p3;
    end else if ((ap_const_logic_1 == ap_sig_cseq_ST_st5_fsm_4)) begin
        code1_reg_142 <= code1_trunc_ext_fu_240_p1;
    end
end

always @ (posedge ap_clk) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st12_fsm_11)) begin
        l_reg_130 <= l_1_reg_363;
    end else if ((ap_const_logic_1 == ap_sig_cseq_ST_st5_fsm_4)) begin
        l_reg_130 <= ap_const_lv32_1;
    end
end

always @ (posedge ap_clk) begin
    if (((ap_const_logic_1 == ap_sig_cseq_ST_st7_fsm_6) & ~(tmp_4_fu_258_p2 == ap_const_lv1_0))) begin
        l_1_reg_363 <= l_1_fu_264_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st6_fsm_5)) begin
        tmp_30_reg_349 <= tmp_30_fu_244_p1;
    end
end

always @ (posedge ap_clk) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st8_fsm_7)) begin
        tmp_31_reg_378 <= tmp_31_fu_288_p1;
    end
end

always @ (posedge ap_clk) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st5_fsm_4)) begin
        tmp_66_reg_324[7 : 2] <= tmp_66_fu_176_p2[7 : 2];
        tmp_67_reg_329[7 : 2] <= tmp_67_fu_194_p2[7 : 2];
        tmp_68_reg_334[7 : 2] <= tmp_68_fu_212_p2[7 : 2];
        tmp_69_reg_339 <= tmp_69_fu_234_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st13_fsm_12)) begin
        tmp_73_reg_388 <= tmp_73_fu_315_p2;
    end
end

always @ (ap_start or ap_sig_cseq_ST_st1_fsm_0 or ap_sig_cseq_ST_st5_fsm_4 or ap_sig_cseq_ST_st8_fsm_7 or ap_sig_cseq_ST_st12_fsm_11 or grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_ce0 or ap_sig_cseq_ST_st2_fsm_1 or ap_sig_cseq_ST_st3_fsm_2 or ap_sig_cseq_ST_st4_fsm_3 or ap_sig_cseq_ST_st9_fsm_8 or ap_sig_cseq_ST_st10_fsm_9 or ap_sig_cseq_ST_st11_fsm_10) begin
    if (((ap_const_logic_1 == ap_sig_cseq_ST_st5_fsm_4) | (ap_const_logic_1 == ap_sig_cseq_ST_st8_fsm_7) | (ap_const_logic_1 == ap_sig_cseq_ST_st12_fsm_11) | ((ap_const_logic_1 == ap_sig_cseq_ST_st1_fsm_0) & ~(ap_start == ap_const_logic_0)) | (ap_const_logic_1 == ap_sig_cseq_ST_st2_fsm_1) | (ap_const_logic_1 == ap_sig_cseq_ST_st3_fsm_2) | (ap_const_logic_1 == ap_sig_cseq_ST_st4_fsm_3) | (ap_const_logic_1 == ap_sig_cseq_ST_st9_fsm_8) | (ap_const_logic_1 == ap_sig_cseq_ST_st10_fsm_9) | (ap_const_logic_1 == ap_sig_cseq_ST_st11_fsm_10))) begin
        CurHuffReadBuf_ce0 = grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_ce0;
    end else begin
        CurHuffReadBuf_ce0 = ap_const_logic_0;
    end
end

always @ (ap_sig_cseq_ST_st6_fsm_5) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st6_fsm_5)) begin
        Dhuff_maxcode_ce0 = ap_const_logic_1;
    end else begin
        Dhuff_maxcode_ce0 = ap_const_logic_0;
    end
end

always @ (ap_sig_cseq_ST_st7_fsm_6) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st7_fsm_6)) begin
        Dhuff_mincode_ce0 = ap_const_logic_1;
    end else begin
        Dhuff_mincode_ce0 = ap_const_logic_0;
    end
end

always @ (ap_sig_cseq_ST_st7_fsm_6) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st7_fsm_6)) begin
        Dhuff_valptr_ce0 = ap_const_logic_1;
    end else begin
        Dhuff_valptr_ce0 = ap_const_logic_0;
    end
end

always @ (ap_sig_cseq_ST_st14_fsm_13) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st14_fsm_13)) begin
        Xhuff_huffval_ce0 = ap_const_logic_1;
    end else begin
        Xhuff_huffval_ce0 = ap_const_logic_0;
    end
end

always @ (ap_start or ap_sig_cseq_ST_st1_fsm_0 or ap_sig_cseq_ST_st15_fsm_14) begin
    if (((~(ap_const_logic_1 == ap_start) & (ap_const_logic_1 == ap_sig_cseq_ST_st1_fsm_0)) | (ap_const_logic_1 == ap_sig_cseq_ST_st15_fsm_14))) begin
        ap_done = ap_const_logic_1;
    end else begin
        ap_done = ap_const_logic_0;
    end
end

always @ (ap_start or ap_sig_cseq_ST_st1_fsm_0) begin
    if ((~(ap_const_logic_1 == ap_start) & (ap_const_logic_1 == ap_sig_cseq_ST_st1_fsm_0))) begin
        ap_idle = ap_const_logic_1;
    end else begin
        ap_idle = ap_const_logic_0;
    end
end

always @ (ap_sig_cseq_ST_st15_fsm_14) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st15_fsm_14)) begin
        ap_ready = ap_const_logic_1;
    end else begin
        ap_ready = ap_const_logic_0;
    end
end

always @ (Xhuff_huffval_q0 or ap_return_preg or ap_sig_cseq_ST_st15_fsm_14) begin
    if ((ap_const_logic_1 == ap_sig_cseq_ST_st15_fsm_14)) begin
        ap_return = Xhuff_huffval_q0;
    end else begin
        ap_return = ap_return_preg;
    end
end

always @ (ap_sig_bdd_253) begin
    if (ap_sig_bdd_253) begin
        ap_sig_cseq_ST_st10_fsm_9 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st10_fsm_9 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_262) begin
    if (ap_sig_bdd_262) begin
        ap_sig_cseq_ST_st11_fsm_10 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st11_fsm_10 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_145) begin
    if (ap_sig_bdd_145) begin
        ap_sig_cseq_ST_st12_fsm_11 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st12_fsm_11 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_154) begin
    if (ap_sig_bdd_154) begin
        ap_sig_cseq_ST_st13_fsm_12 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st13_fsm_12 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_163) begin
    if (ap_sig_bdd_163) begin
        ap_sig_cseq_ST_st14_fsm_13 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st14_fsm_13 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_441) begin
    if (ap_sig_bdd_441) begin
        ap_sig_cseq_ST_st15_fsm_14 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st15_fsm_14 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_33) begin
    if (ap_sig_bdd_33) begin
        ap_sig_cseq_ST_st1_fsm_0 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st1_fsm_0 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_213) begin
    if (ap_sig_bdd_213) begin
        ap_sig_cseq_ST_st2_fsm_1 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st2_fsm_1 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_222) begin
    if (ap_sig_bdd_222) begin
        ap_sig_cseq_ST_st3_fsm_2 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st3_fsm_2 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_231) begin
    if (ap_sig_bdd_231) begin
        ap_sig_cseq_ST_st4_fsm_3 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st4_fsm_3 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_89) begin
    if (ap_sig_bdd_89) begin
        ap_sig_cseq_ST_st5_fsm_4 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st5_fsm_4 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_106) begin
    if (ap_sig_bdd_106) begin
        ap_sig_cseq_ST_st6_fsm_5 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st6_fsm_5 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_117) begin
    if (ap_sig_bdd_117) begin
        ap_sig_cseq_ST_st7_fsm_6 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st7_fsm_6 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_136) begin
    if (ap_sig_bdd_136) begin
        ap_sig_cseq_ST_st8_fsm_7 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st8_fsm_7 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_244) begin
    if (ap_sig_bdd_244) begin
        ap_sig_cseq_ST_st9_fsm_8 = ap_const_logic_1;
    end else begin
        ap_sig_cseq_ST_st9_fsm_8 = ap_const_logic_0;
    end
end

always @ (ap_sig_bdd_206) begin
    if (ap_sig_bdd_206) begin
        ap_sig_nseq_ST_st8_fsm_7 = ap_const_logic_1;
    end else begin
        ap_sig_nseq_ST_st8_fsm_7 = ap_const_logic_0;
    end
end

always @ (ap_sig_cseq_ST_st1_fsm_0 or current_read_byte_i or ap_sig_cseq_ST_st5_fsm_4 or ap_sig_cseq_ST_st8_fsm_7 or ap_sig_cseq_ST_st12_fsm_11 or grp_decode_block_buf_getb_fu_152_current_read_byte_o or grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld or ap_sig_cseq_ST_st2_fsm_1 or ap_sig_cseq_ST_st3_fsm_2 or ap_sig_cseq_ST_st4_fsm_3 or ap_sig_cseq_ST_st9_fsm_8 or ap_sig_cseq_ST_st10_fsm_9 or ap_sig_cseq_ST_st11_fsm_10) begin
    if ((((ap_const_logic_1 == ap_sig_cseq_ST_st1_fsm_0) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st2_fsm_1) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st3_fsm_2) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st4_fsm_3) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st5_fsm_4) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st8_fsm_7) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st9_fsm_8) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st10_fsm_9) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st11_fsm_10) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st12_fsm_11) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld)))) begin
        current_read_byte_o = grp_decode_block_buf_getb_fu_152_current_read_byte_o;
    end else begin
        current_read_byte_o = current_read_byte_i;
    end
end

always @ (ap_start or ap_sig_cseq_ST_st1_fsm_0 or grp_decode_block_buf_getb_fu_152_ap_start_ap_start_reg) begin
    if (((ap_const_logic_1 == ap_sig_cseq_ST_st1_fsm_0) & ~(ap_start == ap_const_logic_0))) begin
        grp_decode_block_buf_getb_fu_152_ap_start = ap_const_logic_1;
    end else begin
        grp_decode_block_buf_getb_fu_152_ap_start = grp_decode_block_buf_getb_fu_152_ap_start_ap_start_reg;
    end
end

always @ (ap_sig_cseq_ST_st1_fsm_0 or off_i or ap_sig_cseq_ST_st5_fsm_4 or ap_sig_cseq_ST_st8_fsm_7 or ap_sig_cseq_ST_st12_fsm_11 or grp_decode_block_buf_getb_fu_152_off_o or grp_decode_block_buf_getb_fu_152_off_o_ap_vld or ap_sig_cseq_ST_st2_fsm_1 or ap_sig_cseq_ST_st3_fsm_2 or ap_sig_cseq_ST_st4_fsm_3 or ap_sig_cseq_ST_st9_fsm_8 or ap_sig_cseq_ST_st10_fsm_9 or ap_sig_cseq_ST_st11_fsm_10) begin
    if ((((ap_const_logic_1 == ap_sig_cseq_ST_st1_fsm_0) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st2_fsm_1) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st3_fsm_2) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st4_fsm_3) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st5_fsm_4) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st8_fsm_7) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st9_fsm_8) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st10_fsm_9) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st11_fsm_10) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st12_fsm_11) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_off_o_ap_vld)))) begin
        off_o = grp_decode_block_buf_getb_fu_152_off_o;
    end else begin
        off_o = off_i;
    end
end

always @ (ap_sig_cseq_ST_st1_fsm_0 or read_position_i or ap_sig_cseq_ST_st5_fsm_4 or ap_sig_cseq_ST_st8_fsm_7 or ap_sig_cseq_ST_st12_fsm_11 or grp_decode_block_buf_getb_fu_152_read_position_o or grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld or ap_sig_cseq_ST_st2_fsm_1 or ap_sig_cseq_ST_st3_fsm_2 or ap_sig_cseq_ST_st4_fsm_3 or ap_sig_cseq_ST_st9_fsm_8 or ap_sig_cseq_ST_st10_fsm_9 or ap_sig_cseq_ST_st11_fsm_10) begin
    if ((((ap_const_logic_1 == ap_sig_cseq_ST_st1_fsm_0) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st2_fsm_1) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st3_fsm_2) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st4_fsm_3) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st5_fsm_4) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st8_fsm_7) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st9_fsm_8) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st10_fsm_9) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st11_fsm_10) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)) | ((ap_const_logic_1 == ap_sig_cseq_ST_st12_fsm_11) & (ap_const_logic_1 == grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld)))) begin
        read_position_o = grp_decode_block_buf_getb_fu_152_read_position_o;
    end else begin
        read_position_o = read_position_i;
    end
end
always @ (ap_start or ap_CS_fsm or tmp_4_fu_258_p2) begin
    case (ap_CS_fsm)
        ap_ST_st1_fsm_0 : 
        begin
            if (~(ap_start == ap_const_logic_0)) begin
                ap_NS_fsm = ap_ST_st2_fsm_1;
            end else begin
                ap_NS_fsm = ap_ST_st1_fsm_0;
            end
        end
        ap_ST_st2_fsm_1 : 
        begin
            ap_NS_fsm = ap_ST_st3_fsm_2;
        end
        ap_ST_st3_fsm_2 : 
        begin
            ap_NS_fsm = ap_ST_st4_fsm_3;
        end
        ap_ST_st4_fsm_3 : 
        begin
            ap_NS_fsm = ap_ST_st5_fsm_4;
        end
        ap_ST_st5_fsm_4 : 
        begin
            ap_NS_fsm = ap_ST_st6_fsm_5;
        end
        ap_ST_st6_fsm_5 : 
        begin
            ap_NS_fsm = ap_ST_st7_fsm_6;
        end
        ap_ST_st7_fsm_6 : 
        begin
            if ((tmp_4_fu_258_p2 == ap_const_lv1_0)) begin
                ap_NS_fsm = ap_ST_st13_fsm_12;
            end else begin
                ap_NS_fsm = ap_ST_st8_fsm_7;
            end
        end
        ap_ST_st8_fsm_7 : 
        begin
            ap_NS_fsm = ap_ST_st9_fsm_8;
        end
        ap_ST_st9_fsm_8 : 
        begin
            ap_NS_fsm = ap_ST_st10_fsm_9;
        end
        ap_ST_st10_fsm_9 : 
        begin
            ap_NS_fsm = ap_ST_st11_fsm_10;
        end
        ap_ST_st11_fsm_10 : 
        begin
            ap_NS_fsm = ap_ST_st12_fsm_11;
        end
        ap_ST_st12_fsm_11 : 
        begin
            ap_NS_fsm = ap_ST_st6_fsm_5;
        end
        ap_ST_st13_fsm_12 : 
        begin
            ap_NS_fsm = ap_ST_st14_fsm_13;
        end
        ap_ST_st14_fsm_13 : 
        begin
            ap_NS_fsm = ap_ST_st15_fsm_14;
        end
        ap_ST_st15_fsm_14 : 
        begin
            ap_NS_fsm = ap_ST_st1_fsm_0;
        end
        default : 
        begin
            ap_NS_fsm = 'bx;
        end
    endcase
end


assign CurHuffReadBuf_address0 = grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_address0;

assign Dhuff_maxcode_address0 = tmp_78_cast_fu_253_p1;

assign Dhuff_mincode_address0 = tmp_79_cast_fu_274_p1;

assign Dhuff_valptr_address0 = tmp_80_cast_fu_283_p1;

assign Xhuff_huffval_address0 = tmp_81_cast_fu_320_p1;


always @ (ap_CS_fsm) begin
    ap_sig_bdd_106 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_5]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_117 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_6]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_136 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_7]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_145 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_B]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_154 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_C]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_163 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_D]);
end


always @ (ap_NS_fsm) begin
    ap_sig_bdd_206 = (ap_const_lv1_1 == ap_NS_fsm[ap_const_lv32_7]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_213 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_1]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_222 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_2]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_231 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_3]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_244 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_8]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_253 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_9]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_262 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_A]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_33 = (ap_CS_fsm[ap_const_lv32_0] == ap_const_lv1_1);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_441 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_E]);
end


always @ (ap_CS_fsm) begin
    ap_sig_bdd_89 = (ap_const_lv1_1 == ap_CS_fsm[ap_const_lv32_4]);
end

assign code1_trunc_ext_fu_240_p1 = grp_decode_block_buf_getb_fu_152_ap_return;

assign code_1_fu_292_p3 = {{tmp_31_reg_378}, {grp_decode_block_buf_getb_fu_152_ap_return}};

assign current_read_byte_o_ap_vld = grp_decode_block_buf_getb_fu_152_current_read_byte_o_ap_vld;

assign grp_decode_block_buf_getb_fu_152_CurHuffReadBuf_q0 = CurHuffReadBuf_q0;

assign grp_decode_block_buf_getb_fu_152_ap_ce = ap_const_logic_1;

assign grp_decode_block_buf_getb_fu_152_current_read_byte_i = current_read_byte_i;

assign grp_decode_block_buf_getb_fu_152_off_i = off_i;

assign grp_decode_block_buf_getb_fu_152_read_position_i = read_position_i;

assign l_1_fu_264_p2 = (ap_const_lv32_1 + l_reg_130);

assign off_o_ap_vld = grp_decode_block_buf_getb_fu_152_off_o_ap_vld;

assign p_fu_305_p2 = (tmp_24_fu_299_p2 - Dhuff_mincode_q0);

assign p_shl_cast_fu_226_p3 = {{tmp_29_fu_222_p1}, {ap_const_lv8_0}};

assign read_position_o_ap_vld = grp_decode_block_buf_getb_fu_152_read_position_o_ap_vld;

assign tmp_23_fu_170_p2 = tmp_3 << ap_const_lv8_2;

assign tmp_24_fu_299_p2 = (code1_reg_142 + Dhuff_valptr_q0);

assign tmp_25_fu_182_p2 = tmp_2 << ap_const_lv8_5;

assign tmp_26_fu_188_p2 = tmp_2 << ap_const_lv8_2;

assign tmp_27_fu_200_p2 = tmp_1 << ap_const_lv8_5;

assign tmp_28_fu_206_p2 = tmp_1 << ap_const_lv8_2;

assign tmp_29_fu_222_p1 = tmp_s[2:0];

assign tmp_30_fu_244_p1 = l_reg_130[7:0];

assign tmp_31_fu_288_p1 = code1_reg_142[30:0];

assign tmp_32_fu_311_p1 = p_fu_305_p2[10:0];

assign tmp_4_fu_258_p2 = ($signed(code1_reg_142) > $signed(Dhuff_maxcode_q0)? 1'b1: 1'b0);

assign tmp_66_fu_176_p2 = (tmp_fu_164_p2 + tmp_23_fu_170_p2);

assign tmp_67_fu_194_p2 = (tmp_25_fu_182_p2 + tmp_26_fu_188_p2);

assign tmp_68_fu_212_p2 = (tmp_27_fu_200_p2 + tmp_28_fu_206_p2);

assign tmp_69_fu_234_p2 = ($signed(p_shl_cast_fu_226_p3) + $signed(tmp_cast_cast_fu_218_p1));

assign tmp_70_fu_248_p2 = (tmp_68_reg_334 + tmp_30_fu_244_p1);

assign tmp_71_fu_270_p2 = (tmp_67_reg_329 + tmp_30_reg_349);

assign tmp_72_fu_279_p2 = (tmp_66_reg_324 + tmp_30_reg_349);

assign tmp_73_fu_315_p2 = (tmp_69_reg_339 + tmp_32_fu_311_p1);

assign tmp_78_cast_fu_253_p1 = $signed(tmp_70_fu_248_p2);

assign tmp_79_cast_fu_274_p1 = $signed(tmp_71_fu_270_p2);

assign tmp_80_cast_fu_283_p1 = $signed(tmp_72_fu_279_p2);

assign tmp_81_cast_fu_320_p1 = $signed(tmp_73_reg_388);

assign tmp_cast_cast_fu_218_p1 = $signed(tmp_s);

assign tmp_fu_164_p2 = tmp_3 << ap_const_lv8_5;
always @ (posedge ap_clk) begin
    tmp_66_reg_324[1:0] <= 2'b00;
    tmp_67_reg_329[1:0] <= 2'b00;
    tmp_68_reg_334[1:0] <= 2'b00;
end



endmodule //decode_block_DecodeHuffman

