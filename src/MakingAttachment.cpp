// standard library includes
#include <iostream>
#include <string>

// lolly

#include "string.hpp"


#include <getopt.h>

#include "PDFWriter/PDFWriter.h"
#include "PDFWriter/PDFPage.h"
#include "PDFWriter/PageContentContext.h"
#include "PDFAttachment.h"
#include "PDFAttachmentWriter.h"

#include "PDFWriter/OutputBufferedStream.h"
#include "PDFWriter/OutputFileStream.h"
#include "PDFWriter/InputFileStream.h"

using namespace PDFHummus;

string tmPath;
string pdfPath;
string outPdfPath;
string attachmentName = "sourse.tm";

int main(int argc, char *argv[])
{
    tmPath = PROJECTDIR *string("/29.tm");
    pdfPath = PROJECTDIR * string("/src.pdf");
    outPdfPath = PROJECTDIR * string("/OutputPdf.pdf");
    int c = 0; 
    while(EOF != (c = getopt(argc,argv,"t:p:o:")))
    {
        switch(c)
        {
            case 't':
                std::cout << "tmPath " << optarg << std::endl;
                tmPath = PROJECTDIR * string(optarg);
                break;
            case 'p':
                std::cout << "pdfPath " << optarg << std::endl;
                pdfPath = PROJECTDIR* string(optarg);
                break;
            case 'o':
                std::cout << "outPdfPath " << optarg << std::endl;
                outPdfPath = PROJECTDIR* string(optarg);
                break;
            case 'a':
                std::cout << "attachmentName " << optarg << std::endl;
                attachmentName = optarg;
                break;
            default:
                std::cout << "bad arg" << std::endl;
                exit(0);
                break;
        }    
    }
    PDFWriter pdfWriter;
    EStatusCode status;
    do
    {
        status = pdfWriter.ModifyPDF(as_charp(pdfPath), ePDFVersion16, as_charp(outPdfPath));
        std::cout << as_charp(pdfPath) << std::endl;
        if (status != eSuccess)
        {
            std::cout << "start fail\n"
                 << std::endl;
            break;
        }

        PDFAttachmentWriter attachmentWriter(&pdfWriter);

        InputFileStream tmFileStream;

		status = tmFileStream.Open(as_charp(tmPath));
		if(status != PDFHummus::eSuccess)
		{
			std::cout<<"failed to open 29.tm"<<"\n";
			break;
		}
        LongFilePositionType fileSize = tmFileStream.GetFileSize();
        std::cout << "file Size " << fileSize << std::endl;
        Byte *test_ = new Byte[fileSize + 16];
        tmFileStream.Read(test_, fileSize);
        PDFAttachment *aAttachment = new PDFAttachment(test_, fileSize);
        status = attachmentWriter.AttachToAllPage(aAttachment);
        if (status != eSuccess)
        {
            std::cout << "Attach fail\n"
                 << std::endl;
            break;
        }

        status = pdfWriter.EndPDF();
        delete test_;
    } while (false);

    if (eSuccess == status)
        std::cout << "Succeeded in creating PDF file\n";
    else
        std::cout << "Failed in creating PDF file\n";

    return 0;
}