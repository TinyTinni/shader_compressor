#define _CRT_SECURE_NO_WARNINGS

/*
 Requires:  Windows 8 or Windows Server 2012 or newer
 see here: "Compressapi.h on Windows 8 and Windows Server 2012" ( https://msdn.microsoft.com/en-us/library/windows/desktop/hh437570(v=vs.85).aspx )
*/

#include <Windows.h>
#include <compressapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment (lib, "Cabinet.lib")


int main(int argc, char** argv)
{
    const float maxRatio = 1.f;
    if (argc < 3)
    {
        printf("Please call \"compress <inputfile> <outputfile>\"");
        return 1;
    }

    const char* inFileName = argv[1];
    const char* outFileName = argv[2];

    PVOID inData, outData = NULL;
    DWORD inDataSize, outDataSize = -1;

    HANDLE inF = CreateFile(
        inFileName, GENERIC_READ,  FILE_SHARE_READ,          
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    LARGE_INTEGER li;
    GetFileSizeEx(inF, &li);
    inDataSize = li.LowPart;

    inData = malloc(inDataSize);
    ReadFile(inF, inData, inDataSize, NULL, NULL);
    CloseHandle(inF);

    int outMethod = 0;

    /*
    COMPRESS_ALGORITHM_MSZIP        2
    COMPRESS_ALGORITHM_XPRESS       3
    COMPRESS_ALGORITHM_XPRESS_HUFF  4
    COMPRESS_ALGORITHM_LZMS         5
    */
    for (int m = 2; m <= 5; ++m)
    {
        COMPRESSOR_HANDLE compressor;
        PVOID tmp = malloc(inDataSize);
        SIZE_T tmpSize;

        /* Create Compressor with corresponding method*/
        CreateCompressor(m,
            NULL,
            &compressor);

        BOOL suc = Compress(compressor,
            inData,
            inDataSize,
            tmp,
            inDataSize,
            &tmpSize);

        /* Select best method */
        float ratio = (float)tmpSize / inDataSize;
        if (suc && tmpSize < outDataSize && ratio < maxRatio)
        {
            free(outData);
            outData = tmp;
            outDataSize = (DWORD)tmpSize;
            outMethod = m;
        }
        else
        {
            free(tmp);
        }

        CloseCompressor(compressor);
        
        printf("Use method %d, ratio: %f\n", m, ratio);

    }

    free(inData);

    /* write config output file*/
    if (outMethod)
    {
        FILE* outFile = fopen(outFileName, "wb");

        fprintf(outFile, "#pragma once\n\n");

        fprintf(outFile, "#define COMPRESS_METHOD %d\n\n", outMethod);
        
        fprintf(outFile, "static const char* strcmpr = \"");
        for (SIZE_T i = 0; i < outDataSize; ++i)
        {
            unsigned char* cur = (char*)outData + i;
            unsigned int c = *cur;
            fprintf(outFile, "\\x%x", c);
        }
        fprintf(outFile, "\";\n\n");

        // use unsigned type -> less includes
        fprintf(outFile, "static const unsigned strcmprSize = %d;\n", outDataSize);
        fprintf(outFile, "static const unsigned strdecmprSize = %d;\n", inDataSize);
        fprintf(outFile, "static char strout[strdecmprSize+1];\n");

        fclose(outFile);
    }
    else
        printf("Skipping Compression\n");

    free(outData);

    return (outMethod)?0:1;
}