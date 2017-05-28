#pragma once

/*
include output from compress first
*/

#include <Windows.h>
#include <compressapi.h>

#pragma comment (lib, "Cabinet.lib")

#include <array>

inline void read()
{
    DECOMPRESSOR_HANDLE decompressor;
    CreateDecompressor(COMPRESS_METHOD, NULL, &decompressor);

    Decompress(decompressor, strcmpr, strcmprSize, strout, strdecmprSize, NULL);
    strout[strdecmprSize] = '\0';
    CloseCompressor(decompressor);
}