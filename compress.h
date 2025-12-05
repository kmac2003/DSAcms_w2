/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			MICHAEL GALLE
Implemented:    KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

				Huffman / RLE compression implementation file

==========================================================================================================================
*/
#pragma once
//*******************************************************************************************************************************    HUFFMAN
//function prototypes
int Huffman_Compress(unsigned char* in, unsigned char* out, unsigned int insize);
void Huffman_Uncompress(unsigned char* in, unsigned char* out, unsigned int insize, unsigned int outsize);

//file wrapper functions
void huffman_compress_file(const char* input_file, const char* output_file);
void huffman_decompress_file(const char* input_file, const char* output_file);

//*******************************************************************************************************************************    RLE
// Original RLE functions (from provided rle.c)
int RLE_Compress(unsigned char* in, unsigned char* out, unsigned int insize);
void RLE_Uncompress(unsigned char* in, unsigned char* out, unsigned int insize);

//RLE encrpyt and decrypt
int RLE_Encode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc);
int RLE_Decode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc);

// File wrapper functions
void rle_compress_file(const char* input_file, const char* output_file);
void rle_decompress_file(const char* input_file, const char* output_file);

//w6
int compressTextRLE(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);
int decompressTextRLE(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);
int compressAudioRLE(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);
int decompressAudioRLE(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);

//w7
int compressTextHuffman(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);
int decompressTextHuffman(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);
int compressAudioHuffman(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);
int decompressAudioHuffman(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputMaxSize);