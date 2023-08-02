
#include "PDFWriter/PDFWriter.h"
#include "PDFWriter/DictionaryContext.h"
#include "PDFWriter/ObjectsContext.h"
#include "PDFWriter/PDFRectangle.h"
#include "PDFWriter/Trace.h"
#include "PDFWriter/SafeBufferMacrosDefs.h"
#include "PDFWriter/PDFStream.h"

#include "PDFWriter/OutputBufferedStream.h"
#include "PDFWriter/OutputFileStream.h"
#include "PDFWriter/InputFileStream.h"
using namespace PDFHummus;
#include <utility>

using namespace IOBasicTypes;
#include "iterator.hpp"
#include "string.hpp"
#include "hashmap.hpp"

#include "pdf_hummus_make_attachment.hpp"

class PDFAttachment
{
public:
    PDFAttachment(void);
    PDFAttachment(Byte inByte[], size_t inLenth);
    ~PDFAttachment(void);

    Byte *FileContent;
    size_t Lenth;
};

class PDFWriter;

typedef hashmap<PDFAttachment *, ObjectIDType> PDFAttachmentToObjectIDTypeMap;
//typedef pair<EStatusCode, ObjectIDType> EStatusCodeAndObjectIDType;

class PDFAttachmentWriter : public DocumentContextExtenderAdapter
{
public:
    PDFAttachmentWriter(PDFWriter *inPDFWriter);
    ~PDFAttachmentWriter(void);

    EStatusCode AttachToAllPage(PDFAttachment *inAttachment);

    virtual EStatusCode OnCatalogWrite(
        CatalogInformation *inCatalogInformation,
        DictionaryContext *inCatalogDictionaryContext,
        ObjectsContext *inPDFWriterObjectContext,
        DocumentContext *inPDFWriterDocumentContext);

private:
    PDFWriter *mPDFWriter;
    PDFAttachmentToObjectIDTypeMap mAttachment;
    void ListenOnCatalogWrite();
    EStatusCodeAndObjectIDType WriteAttachment(PDFAttachment *inAttachment);
    void CleanupAttachment();
};
string attachment_name;
void pdf_hummus_make_attachment (string pdf_path, string attachment_path, string out_path)
{
    int i, last_slash_index = 0;
    for(i = 0; i < N(attachment_path) ;i ++)
    {
        if(attachment_path[i] == '/')
            last_slash_index = i;
    }
    attachment_name = attachment_path(last_slash_index + 1, N(attachment_path));
    //std::cout << last_slash_index << " " << N(attachment_path) << " " << as_charp(attachment_name) << std::endl;
    //return;
    PDFWriter pdfWriter;
    EStatusCode status;
    do
    {
        status = pdfWriter.ModifyPDF(as_charp(pdf_path), ePDFVersion16, as_charp(out_path));
        std::cout << as_charp(pdf_path)
                 << std::endl;
        std::cout << as_charp(out_path)
                 << std::endl;
        if (status != eSuccess)
        {
            std::cout << "start fail"
                 << std::endl;
            break;
        }
        //return;
        PDFAttachmentWriter attachmentWriter(&pdfWriter);

        InputFileStream tm_file_stream;

		status = tm_file_stream.Open(as_charp(attachment_path));
		if(status != PDFHummus::eSuccess)
		{
			std::cout<<"failed to open tm"<<std::endl;
			break;
		}
        LongFilePositionType fileSize = tm_file_stream.GetFileSize();
        std::cout << "file Size " << fileSize << std::endl;

        //new 要替换成tm_new
        Byte *test_ = new Byte[fileSize + 16];
        tm_file_stream.Read(test_, fileSize);
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

    //return 0;
}
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



PDFAttachmentWriter::PDFAttachmentWriter(PDFWriter *inPDFWriter)
{
    mPDFWriter = inPDFWriter;
    ListenOnCatalogWrite();
}

void PDFAttachmentWriter::ListenOnCatalogWrite()
{
    mPDFWriter->GetDocumentContext().AddDocumentContextExtender(this);
}

PDFAttachmentWriter::~PDFAttachmentWriter(void)
{
    if (N(mAttachment) != 0)
        TRACE_LOG("PDFAttachmentWriter::~PDFAttachmentWriter, Exception. Has comments that were not associated with a page");

    CleanupAttachment();
}

EStatusCode PDFAttachmentWriter::OnCatalogWrite(
    CatalogInformation *inCatalogInformation,
    DictionaryContext *inCatalogDictionaryContext,
    ObjectsContext *inPDFWriterObjectContext,
    DocumentContext *inPDFWriterDocumentContext)
{
    if (N(mAttachment) == 0)
        return eSuccess;

    inCatalogDictionaryContext->WriteKey("Names");

    iterator<PDFAttachment *> it= iterate (mAttachment);
    while (it->busy ()) {

        DictionaryContext *dictionaryContext_0 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_0->WriteKey("EmbeddedFiles");
        DictionaryContext *dictionaryContext_1 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_0->WriteKey("Names");
        inPDFWriterObjectContext->StartArray();
        inPDFWriterObjectContext->WriteLiteralString(as_charp(attachment_name));
        DictionaryContext *dictionaryContext_2 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_2->WriteKey("EF");
        DictionaryContext *dictionaryContext_3 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_3->WriteKey("F");
        inPDFWriterObjectContext->WriteIndirectObjectReference(mAttachment[it->next()]);
        inPDFWriterObjectContext->EndDictionary(dictionaryContext_3);
        dictionaryContext_2->WriteKey("F");
        dictionaryContext_2->WriteLiteralStringValue(as_charp(attachment_name));
        dictionaryContext_2->WriteKey("Type");
        dictionaryContext_2->WriteNameValue("F");
        inPDFWriterObjectContext->EndDictionary(dictionaryContext_2);
        inPDFWriterObjectContext->EndArray();
        inPDFWriterObjectContext->EndDictionary(dictionaryContext_1);
        inPDFWriterObjectContext->EndDictionary(dictionaryContext_0);
    }
    CleanupAttachment();
    return eSuccess;
}

void PDFAttachmentWriter::CleanupAttachment()
{
    
    iterator<PDFAttachment *> it= iterate (mAttachment);
    while (it->busy ())
    {
        delete it->next();
    }
    mAttachment = PDFAttachmentToObjectIDTypeMap();
}

EStatusCode PDFAttachmentWriter::AttachToAllPage(PDFAttachment *inAttachment)
{
    return WriteAttachment(inAttachment).first;
}

EStatusCodeAndObjectIDType PDFAttachmentWriter::WriteAttachment(PDFAttachment *inAttachment)
{
    EStatusCodeAndObjectIDType result;

    if(mAttachment->contains(inAttachment))
    {
        result.first = eSuccess;
        result.second = mAttachment[inAttachment];
        return result;
    }

    do
    {
        ObjectsContext &objectsContext = mPDFWriter->GetObjectsContext();

        // Start new InDirect object for annotation dictionary
        result.second = objectsContext.StartNewIndirectObject();
        DictionaryContext *dictionaryContext = objectsContext.StartDictionary();
        dictionaryContext->WriteKey("Type");
        dictionaryContext->WriteNameValue("EmbeddedFile");
        PDFStream *pdfStream = objectsContext.StartPDFStream(dictionaryContext);
        IByteWriter *mwriter = pdfStream->GetWriteStream();
        mwriter->Write(inAttachment->FileContent, inAttachment->Lenth);
        objectsContext.EndPDFStream(pdfStream);

        //mAttachment.insert(PDFAttachmentToObjectIDTypeMap::value_type(inAttachment, result.second));
        mAttachment(inAttachment) = result.second;
        result.first = eSuccess;
    } while (false);

    return result;
}
