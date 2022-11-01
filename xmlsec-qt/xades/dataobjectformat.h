#ifndef  XMLSECQTXADESOBJCETDATAOBJECTFORMAT_H
# define XMLSECQTXADESOBJCETDATAOBJECTFORMAT_H

# include "context.h"

class QXadesDataObjectFormat
{
public:
  QXadesDataObjectFormat(const QString& referenceId) : referenceId(referenceId) {}

  QXadesDataObjectFormat& withDescription(const QString& val) { description = val; return *this; }
  QXadesDataObjectFormat& withIdentifierDescription(const QString& val) { identifierDescription = val; return *this; }
  QXadesDataObjectFormat& withIdentifierQualifier(const QString& val) { identifierQualifier = val; return *this; }
  QXadesDataObjectFormat& withIdentifier(const QString& val)  { identifier  = val; return *this; }
  QXadesDataObjectFormat& withMimetype(const QString& val)    { mimetype = val; return *this; }
  QXadesDataObjectFormat& withEncoding(const QString& val)    { encoding = val; return *this; }

  QDomElement generate(QXadesContext& context) const
  {
    QDomElement root          = context.createElement("DataObjectFormat");
    QDomElement descriptionEl = context.createElement("Description");
    QDomElement mimetypeEl    = context.createElement("MimeType");
    QDomElement encodingEl    = context.createElement("Encoding");

    if (description.length() > 0)
      descriptionEl.appendChild(context.document.createTextNode(description));
    if (encoding.length() > 0)
      encodingEl.appendChild(context.document.createTextNode(encoding));
    if (mimetype.length() > 0)
      mimetypeEl.appendChild(context.document.createTextNode(mimetype));
    root.setAttribute("ObjectReference", '#' + referenceId);
    root.appendChild(descriptionEl);
    root.appendChild(generateObjectIdentifier(context));
    root.appendChild(mimetypeEl);
    root.appendChild(encodingEl);
    return root;
  }

private:
  QDomElement generateObjectIdentifier(QXadesContext& context) const
  {
    QDomElement root = context.createElement("ObjectIdentifier");
    QDomElement identifierEl = context.createElement("Identifier");
    QDomElement descriptionEl = context.createElement("Description");

    if (identifierQualifier.length() > 0)
      identifierEl.setAttribute("Qualifier", identifierQualifier);
    if (identifierDescription.length() > 0)
      descriptionEl.appendChild(context.document.createTextNode(identifierDescription));
    identifierEl.appendChild(context.document.createTextNode(identifier));
    root.appendChild(identifierEl);
    root.appendChild(descriptionEl);
    return root;
  }

  QString referenceId, description;
  QString identifier, identifierQualifier, identifierDescription;
  QString mimetype, encoding;
};

#endif
