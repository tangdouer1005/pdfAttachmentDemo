// standard library includes
#include <iostream>
#include <string>
using namespace std;
// end standard library includes

// pdfwriter library includes
#include "PDFWriter/PDFWriter.h"
#include "PDFWriter/PDFPage.h"
#include "PDFWriter/PageContentContext.h"
#include "PDFAttachment.h"
#include "PDFAttachmentWriter.h"

#include "PDFWriter/OutputBufferedStream.h"
#include "PDFWriter/OutputFileStream.h"
#include "PDFWriter/InputFileStream.h"

using namespace PDFHummus;


int main(int argc, char *argv[])
{
    PDFWriter pdfWriter;
    EStatusCode status;

    do
    {
        // status = pdfWriter.StartPDF(scBasePath + "MakingComments.pdf", ePDFVersion16);
        status = pdfWriter.ModifyPDF("26_2_2pages.pdf", ePDFVersion16, "OutputPdf.pdf");
        if (status != eSuccess)
        {
            cout << "start fail\n"
                 << endl;
            break;
        }

        // // Create a new page
        // PDFPage *pdfPage = new PDFPage();
        // pdfPage->SetMediaBox(PDFRectangle(0, 0, 595, 842));

        // // Create a content context for the page
        // PageContentContext *pageContentContext = pdfWriter.StartPageContentContext(pdfPage);

        PDFAttachmentWriter attachmentWriter(&pdfWriter);

        InputFileStream tmFileStream;

		status = tmFileStream.Open("29.tm");
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

        // status = pdfWriter.WritePageAndRelease(pdfPage);
        // if (status != eSuccess)
        // {
        //     cout << "WritePageAndRelease fail\n"
        //          << endl;
        //     break;
        // }

        status = pdfWriter.EndPDF();
    } while (false);

    if (eSuccess == status)
        cout << "Succeeded in creating PDF file\n";
    else
        cout << "Failed in creating PDF file\n";

    return 0;
}