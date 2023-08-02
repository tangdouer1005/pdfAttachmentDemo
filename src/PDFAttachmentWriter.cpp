#include "PDFAttachmentWriter.h"
#include "PDFWriter/PDFWriter.h"
#include "PDFWriter/DictionaryContext.h"
#include "PDFWriter/ObjectsContext.h"
#include "PDFWriter/PDFRectangle.h"
#include "PDFWriter/Trace.h"
#include "PDFWriter/SafeBufferMacrosDefs.h"
#include "PDFWriter/PDFStream.h"

using namespace PDFHummus;
extern string attachmentName;

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
    // write the comments as the page array of annotations

    if (N(mAttachment) == 0)
        return eSuccess;

    // if (inCatalogDictionaryContext->WriteKey("Names") != eSuccess)
    // {
    //     // Failed to write Annots key! there's already an annots entry. hmm. need to coordinate with another option
    //     TRACE_LOG("PDFAttachmentWriter::OnCatalogWrite, Exception. OnCatalogWrite already written for this page, can't write a new entr");
    //     return eFailure;
    // }

    //DictionaryContext *dictionaryContext = inPDFWriterObjectContext->StartDictionary();
    inCatalogDictionaryContext->WriteKey("Names");
    // PDFAttachmentToObjectIDTypeMap::iterator it = mAttachment.begin();
    // for (; it != mAttachment.end(); ++it)
    // {
    //     DictionaryContext *dictionaryContext_0 = inPDFWriterObjectContext->StartDictionary();
    //     dictionaryContext_0->WriteKey("EmbeddedFiles");
    //     DictionaryContext *dictionaryContext_1 = inPDFWriterObjectContext->StartDictionary();
    //     dictionaryContext_0->WriteKey("Names");
    //     inPDFWriterObjectContext->StartArray();
    //     inPDFWriterObjectContext->WriteLiteralString(attachmentName);
    //     DictionaryContext *dictionaryContext_2 = inPDFWriterObjectContext->StartDictionary();

    //     dictionaryContext_2->WriteKey("EF");
    //     DictionaryContext *dictionaryContext_3 = inPDFWriterObjectContext->StartDictionary();
    //     dictionaryContext_3->WriteKey("F");
    //     inPDFWriterObjectContext->WriteIndirectObjectReference(it->second);
    //     inPDFWriterObjectContext->EndDictionary(dictionaryContext_3);
    //     dictionaryContext_2->WriteKey("F");
    //     dictionaryContext_2->WriteLiteralStringValue(attachmentName);
    //     dictionaryContext_2->WriteKey("Type");
    //     dictionaryContext_2->WriteNameValue("F");
    //     inPDFWriterObjectContext->EndDictionary(dictionaryContext_2);
    //     inPDFWriterObjectContext->EndArray();
    //     inPDFWriterObjectContext->EndDictionary(dictionaryContext_1);
    //     inPDFWriterObjectContext->EndDictionary(dictionaryContext_0);
    // }

    iterator<PDFAttachment *> it= iterate (mAttachment);
    while (it->busy ()) {

        DictionaryContext *dictionaryContext_0 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_0->WriteKey("EmbeddedFiles");
        DictionaryContext *dictionaryContext_1 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_0->WriteKey("Names");
        inPDFWriterObjectContext->StartArray();
        inPDFWriterObjectContext->WriteLiteralString(as_charp(attachmentName));
        DictionaryContext *dictionaryContext_2 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_2->WriteKey("EF");
        DictionaryContext *dictionaryContext_3 = inPDFWriterObjectContext->StartDictionary();
        dictionaryContext_3->WriteKey("F");
        inPDFWriterObjectContext->WriteIndirectObjectReference(mAttachment[it->next()]);
        inPDFWriterObjectContext->EndDictionary(dictionaryContext_3);
        dictionaryContext_2->WriteKey("F");
        dictionaryContext_2->WriteLiteralStringValue(as_charp(attachmentName));
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
    // PDFAttachmentToObjectIDTypeMap::iterator it = mAttachment.begin();
    // for (; it != mAttachment.end(); ++it)
    //     delete it->first;
    //mAttachment.clear();
}

EStatusCode PDFAttachmentWriter::AttachToAllPage(PDFAttachment *inAttachment)
{
    return WriteAttachment(inAttachment).first;
}

EStatusCodeAndObjectIDType PDFAttachmentWriter::WriteAttachment(PDFAttachment *inAttachment)
{
    EStatusCodeAndObjectIDType result;

    // if already written, return

    // PDFAttachmentToObjectIDTypeMap::iterator it = mAttachment.find(inAttachment);
    // if (it != mAttachment.end())
    // {
    //     result.first = eSuccess;
    //     result.second = it->second;
    //     return result;
    // }
    // iterator<PDFAttachment *> it= iterate (mAttachment);
    // while (it->busy ())
    // {
    //     if(it->remains() == inAttachment)
    //         break;
    //     it -> next();
    // }
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
