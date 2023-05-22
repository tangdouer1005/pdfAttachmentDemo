#pragma once

#include "PDFAttachment.h"
#include "ObjectsBasicTypes.h"
#include "DocumentContextExtenderAdapter.h"

#include "OutputBufferedStream.h"
#include "OutputFileStream.h"

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
