
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
#include "tm_ostream.hpp"

#include "pdf_hummus_make_attachment.hpp"

class PDFAttachment
{
public:
    PDFAttachment(void);
    PDFAttachment(Byte inByte[], size_t inLenth, string inName);
    ~PDFAttachment(void);

    Byte *FileContent;
    size_t Lenth;
    string Name;
};

class PDFWriter;

typedef hashmap<PDFAttachment *, ObjectIDType> PDFAttachmentToObjectIDTypeMap;

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
void pdf_hummus_make_attachment (string pdf_path, string attachment_path, string out_path)
{
    PDFWriter pdfWriter;
    EStatusCode status;
    do
    {
        status = pdfWriter.ModifyPDF(as_charp(pdf_path), ePDFVersion16, as_charp(out_path));
        if (status != eSuccess)
        {
            cout << "start fail\n";
            break;
        }
        PDFAttachmentWriter attachmentWriter(&pdfWriter);

        InputFileStream tm_file_stream;
		status = tm_file_stream.Open(as_charp(attachment_path));
		if(status != PDFHummus::eSuccess)
		{
			cout<<"failed to open tm\n";
			break;
		}
        LongFilePositionType file_size = tm_file_stream.GetFileSize();
        Byte *file_content = new Byte[file_size + 16];
        tm_file_stream.Read(file_content, file_size);


        int i, last_slash_index = 0;
        for(i = 0; i < N(attachment_path) ;i ++)
        {
            if(attachment_path[i] == '/')
                last_slash_index = i;
        }
        string attachment_name = attachment_path(last_slash_index + 1, N(attachment_path));
        
        PDFAttachment *aAttachment = new PDFAttachment(file_content, file_size, attachment_name);

        status = attachmentWriter.AttachToAllPage(aAttachment);
        if (status != eSuccess)
        {
            cout << "Attach fail\n";
            break;
        }

        status = pdfWriter.EndPDF();
    } while (false);

    if (eSuccess == status)
        cout << "Succeeded in creating PDF file\n";
    else
        cout << "Failed in creating PDF file\n";
}
PDFAttachment::PDFAttachment(void)
{
    FileContent = NULL;
    Lenth = 0;
}
PDFAttachment::PDFAttachment(Byte inByte[], size_t inLenth, string inName)
{
    FileContent = inByte;
    Lenth = inLenth;
    Name = inName;
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
        TRACE_LOG("PDFAttachmentWriter::~PDFAttachmentWriter, Exception. Has attachement that were not associated with pages");
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
        PDFAttachment *cur_attachment = it->next();
        DictionaryContext *dictionaryContext_0 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_0->WriteKey("EmbeddedFiles");
        DictionaryContext *dictionaryContext_1 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_0->WriteKey("Names");
        inPDFWriterObjectContext->StartArray();
        inPDFWriterObjectContext->WriteLiteralString(as_charp(cur_attachment -> Name));
        DictionaryContext *dictionaryContext_2 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_2->WriteKey("EF");
        DictionaryContext *dictionaryContext_3 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_3->WriteKey("F");
        inPDFWriterObjectContext->WriteIndirectObjectReference(mAttachment[cur_attachment]);
        inPDFWriterObjectContext->EndDictionary(dictionaryContext_3);
        dictionaryContext_2->WriteKey("F");
        dictionaryContext_2->WriteLiteralStringValue(as_charp(cur_attachment -> Name));
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
