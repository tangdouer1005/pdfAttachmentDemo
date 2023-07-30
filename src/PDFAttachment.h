#pragma once

// allowing myself a shortcut here for the color type and date type. real life sample should use only model classes...and not of the PDF writing implementation...
#include "PDFWriter/OutputBufferedStream.h"
#include "PDFWriter/OutputFileStream.h"

#include <iostream>

#include <string>
#include <cstring>

using namespace IOBasicTypes;
using namespace std;

class PDFAttachment
{
public:
    PDFAttachment(void);
    PDFAttachment(Byte inByte[], size_t inLenth);
    ~PDFAttachment(void);

    Byte *FileContent;
    size_t Lenth;
};
