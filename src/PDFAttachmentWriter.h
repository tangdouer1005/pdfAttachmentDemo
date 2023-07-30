#pragma once

#include "PDFAttachment.h"
#include "PDFWriter/ObjectsBasicTypes.h"
#include "PDFWriter/DocumentContextExtenderAdapter.h"

#include "PDFWriter/OutputBufferedStream.h"
#include "PDFWriter/OutputFileStream.h"

#include <map>
#include <utility>

using namespace IOBasicTypes;
using namespace std;

class PDFWriter;

typedef map<PDFAttachment *, ObjectIDType> PDFAttachmentToObjectIDTypeMap;
typedef pair<EStatusCode, ObjectIDType> EStatusCodeAndObjectIDType;

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
