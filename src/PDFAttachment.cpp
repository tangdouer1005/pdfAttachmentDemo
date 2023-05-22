#include "PDFAttachment.h"

PDFAttachment::PDFAttachment(void)
{
    FileContent = NULL;
    Lenth = 0;
}
PDFAttachment::PDFAttachment(Byte inByte[], size_t inLenth)
{
    FileContent = new Byte[inLenth];
    memcpy(FileContent, inByte, inLenth);
    Lenth = inLenth;
}

PDFAttachment::~PDFAttachment(void)
{
    if (FileContent)
        delete FileContent;
}
