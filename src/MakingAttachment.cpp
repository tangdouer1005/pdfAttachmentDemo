// standard library includes
#include <iostream>
#include <string>
using namespace std;
// end standard library includes

// pdfwriter library includes
#include "PDFWriter.h"
#include "PDFPage.h"
#include "PageContentContext.h"
#include "PDFAttachment.h"
#include "PDFAttachmentWriter.h"

#include "OutputBufferedStream.h"
#include "OutputFileStream.h"

using namespace PDFHummus;

// end pdfwriter library includes

static const string scBasePath = "../outpdf";
static const string scSystemFontsPath = "../fonts";

int main(int argc, char *argv[])
{
    PDFWriter pdfWriter;
    EStatusCode status;

    do
    {
        // status = pdfWriter.StartPDF(scBasePath + "MakingComments.pdf", ePDFVersion16);
        status = pdfWriter.StartPDF("MakingAttachment.pdf", ePDFVersion16);
        if (status != eSuccess)
        {
            cout << "start fail\n"
                 << endl;
            break;
        }

        // Create a new page
        PDFPage *pdfPage = new PDFPage();
        pdfPage->SetMediaBox(PDFRectangle(0, 0, 595, 842));

        // Create a content context for the page
        PageContentContext *pageContentContext = pdfWriter.StartPageContentContext(pdfPage);

        PDFAttachmentWriter attachmentWriter(&pdfWriter);
        Byte test_[5] = {'a', 'b', 'c', 'd', 'e'};

        PDFAttachment *aAttachment = new PDFAttachment(test_, 5);
        status = attachmentWriter.AttachToAllPage(aAttachment);
        if (status != eSuccess)
        {
            cout << "Attach fail\n"
                 << endl;
            break;
        }

        status = pdfWriter.WritePageAndRelease(pdfPage);
        if (status != eSuccess)
        {
            cout << "WritePageAndRelease fail\n"
                 << endl;
            break;
        }

        status = pdfWriter.EndPDF();
    } while (false);

    if (eSuccess == status)
        cout << "Succeeded in creating PDF file\n";
    else
        cout << "Failed in creating PDF file\n";

    return 0;
}