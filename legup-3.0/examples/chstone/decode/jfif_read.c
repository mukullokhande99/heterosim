/*
+--------------------------------------------------------------------------+
| CHStone : a suite of benchmark programs for C-based High-Level Synthesis |
| ======================================================================== |
|                                                                          |
| * Collected and Modified : Y. Hara, H. Tomiyama, S. Honda,               |
|                            H. Takada and K. Ishii                        |
|                            Nagoya University, Japan                      |
|                                                                          |
| * Remark :                                                               |
|    1. This source code is modified to unify the formats of the benchmark |
|       programs in CHStone.                                               |
|    2. Test vectors are added for CHStone.                                |
|    3. If "main_result" is 0 at the end of the program, the program is    |
|       correctly executed.                                                |
|    4. Please follow the copyright of each benchmark program.             |
+--------------------------------------------------------------------------+
*/
/*
 * Copyright (C) 2008
 * Y. Hara, H. Tomiyama, S. Honda, H. Takada and K. Ishii
 * Nagoya University, Japan
 * All rights reserved.
 *
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any license fee or
 * royalty on an "as is" basis. The authors disclaims any and all warranties, 
 * whether express, implied, or statuary, including any implied warranties or 
 * merchantability or of fitness for a particular purpose. In no event shall the
 * copyright-holder be liable for any incidental, punitive, or consequential damages
 * of any kind whatsoever arising from the use of these programs. This disclaimer
 * of warranty extends to the user of these programs and user's customers, employees,
 * agents, transferees, successors, and assigns.
 *
 */
/*
 * Read the header information from buffer in JFIF format and begin decoding
 *
 *  @(#) $Id: jfif_read.c,v 1.2 2003/07/18 10:19:21 honda Exp $ 
 */

/*
 * Initialize after reading markers
 */

#include "sw.h"
#include "jfif_read.h"



int
huff_make_dhuff_tb (int *p_xhtbl_bits, int p_dhtbl_ml,
		    int *p_dhtbl_maxcode, int *p_dhtbl_mincode,
		    int *p_dhtbl_valptr)
{
  int i, j, p, code, size, l;
  int huffsize[257];
  int huffcode[257];
  int lastp;

  /*
   * Get size
   */
  for (p = 0, i = 1; i < 17; i++)
    {
      for (j = 1; j <= p_xhtbl_bits[i]; j++)
	{
	  huffsize[p++] = i;
	}
    }

  huffsize[p] = 0;
  lastp = p;

  p = 0;
  code = 0;
  size = huffsize[0];
  while (1)
    {
      do
	{
	  huffcode[p++] = code++;
	}
      while ((huffsize[p] == size) && (p < 257));
      /* Overflow Detection */
      if (!huffsize[p])
	{			/* All finished. */
	  break;
	}
      do
	{
	  /* Shift next code to expand prefix. */
	  code <<= 1;
	  size++;
	}
      while (huffsize[p] != size);
    }

  for (p_dhtbl_ml = 1, p = 0, l = 1; l <= 16; l++)
    {
      if (p_xhtbl_bits[l] == 0)
	{
	  p_dhtbl_maxcode[l] = -1;	/* Watch out JPEG is wrong here */
	}
      else
	{			/* We use -1 to indicate skipping. */
	  p_dhtbl_valptr[l] = p;
	  p_dhtbl_mincode[l] = huffcode[p];
	  p += p_xhtbl_bits[l] - 1;
	  p_dhtbl_maxcode[l] = huffcode[p];
	  p_dhtbl_ml = l;
	  p++;
	}
    }
  p_dhtbl_maxcode[p_dhtbl_ml]++;
  return p_dhtbl_ml;
}


void
jpeg_init_decompress ()
{
  int tmp;
  /*
   * Get MCU number
   */
  p_jinfo_MCUHeight = (p_jinfo_image_height - 1) / 8 + 1;
  p_jinfo_MCUWidth = (p_jinfo_image_width - 1) / 8 + 1;
  p_jinfo_NumMCU = p_jinfo_MCUHeight * p_jinfo_MCUWidth;

  /*
   * Create Huffman Table for decoding
   */
  tmp = huff_make_dhuff_tb (&p_jinfo_dc_xhuff_tbl_bits[0][0],
			    p_jinfo_dc_dhuff_tbl_ml[0],
			    &p_jinfo_dc_dhuff_tbl_maxcode[0][0],
			    &p_jinfo_dc_dhuff_tbl_mincode[0][0],
			    &p_jinfo_dc_dhuff_tbl_valptr[0][0]);
  p_jinfo_dc_dhuff_tbl_ml[0] = tmp;
  tmp = huff_make_dhuff_tb (&p_jinfo_dc_xhuff_tbl_bits[1][0],
			    p_jinfo_dc_dhuff_tbl_ml[1],
			    &p_jinfo_dc_dhuff_tbl_maxcode[1][0],
			    &p_jinfo_dc_dhuff_tbl_mincode[1][0],
			    &p_jinfo_dc_dhuff_tbl_valptr[1][0]);
  p_jinfo_dc_dhuff_tbl_ml[1] = tmp;
  tmp = huff_make_dhuff_tb (&p_jinfo_ac_xhuff_tbl_bits[0][0],
			    p_jinfo_ac_dhuff_tbl_ml[0],
			    &p_jinfo_ac_dhuff_tbl_maxcode[0][0],
			    &p_jinfo_ac_dhuff_tbl_mincode[0][0],
			    &p_jinfo_ac_dhuff_tbl_valptr[0][0]);
  p_jinfo_ac_dhuff_tbl_ml[0] = tmp;
  tmp = huff_make_dhuff_tb (&p_jinfo_ac_xhuff_tbl_bits[1][0],
			    p_jinfo_ac_dhuff_tbl_ml[1],
			    &p_jinfo_ac_dhuff_tbl_maxcode[1][0],
			    &p_jinfo_ac_dhuff_tbl_mincode[1][0],
			    &p_jinfo_ac_dhuff_tbl_valptr[1][0]);
  p_jinfo_ac_dhuff_tbl_ml[1] = tmp;
}



void
jpeg_read (unsigned char *read_buf)
{

  /*
   * Read markers
   */
  read_markers (read_buf);


  /*
   * Initialize the information used for decoding
   */
  jpeg_init_decompress ();

  int m,n;
  int a;
  m= 16000;
  n= 16000;
  data_to(buff,m);
  /*
   * Start decoding
   */
  a = decode_start (buff,m,buff,n);

  data_from(buff,n);

}
