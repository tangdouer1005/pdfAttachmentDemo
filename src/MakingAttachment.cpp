// standard library includes
#include <iostream>
#include <string>
#include <getopt.h>
using namespace std;

#include "PDFWriter/PDFWriter.h"
#include "PDFWriter/PDFPage.h"
#include "PDFWriter/PageContentContext.h"
#include "PDFAttachment.h"
#include "PDFAttachmentWriter.h"

#include "PDFWriter/OutputBufferedStream.h"
#include "PDFWriter/OutputFileStream.h"
#include "PDFWriter/InputFileStream.h"

using namespace PDFHummus;

string tmPath = "/29.tm";
string pdfPath = "/src.pdf";
string outPdfPath = "/OutputPdf.pdf";
string attachmentName = "sourse.tm";

int main(int argc, char *argv[])
{
    tmPath = PROJECTDIR +tmPath ;
    pdfPath = PROJECTDIR +pdfPath ;
    outPdfPath = PROJECTDIR +outPdfPath ;
    int c = 0; 
    while(EOF != (c = getopt(argc,argv,"t:p:o:")))
    {
        switch(c)
        {
            case 't':
                cout << "tmPath " << optarg << endl;
                tmPath = PROJECTDIR;
                tmPath += optarg;
                break;
            case 'p':
                cout << "pdfPath " << optarg << endl;
                pdfPath = PROJECTDIR;
                pdfPath += optarg;
                break;
            case 'o':
                cout << "outPdfPath " << optarg << endl;
                outPdfPath = PROJECTDIR;
                outPdfPath += optarg;
                break;
            case 'a':
                cout << "attachmentName " << optarg << endl;
                attachmentName = optarg;
                break;
            default:
                cout << "bad arg" << endl;
                exit(0);
                break;
        }    
    }
    PDFWriter pdfWriter;
    EStatusCode status;
    do
    {
        status = pdfWriter.ModifyPDF(pdfPath, ePDFVersion16, outPdfPath);
        cout << pdfPath << endl;
        if (status != eSuccess)
        {
            cout << "start fail\n"
                 << endl;
            break;
        }

        PDFAttachmentWriter attachmentWriter(&pdfWriter);

        InputFileStream tmFileStream;

		status = tmFileStream.Open(tmPath);
		if(status != PDFHummus::eSuccess)
		{
			cout<<"failed to open 29.tm"<<"\n";
			break;
		}
        LongFilePositionType fileSize = tmFileStream.GetFileSize();
        cout << "file Size " << fileSize << endl;
        Byte *test_ = new Byte[fileSize + 16];
        tmFileStream.Read(test_, fileSize);
        PDFAttachment *aAttachment = new PDFAttachment(test_, fileSize);
        status = attachmentWriter.AttachToAllPage(aAttachment);
        if (status != eSuccess)
        {
            cout << "Attach fail\n"
                 << endl;
            break;
        }

        status = pdfWriter.EndPDF();
        delete test_;
    } while (false);

    if (eSuccess == status)
        cout << "Succeeded in creating PDF file\n";
    else
        cout << "Failed in creating PDF file\n";

    return 0;
}