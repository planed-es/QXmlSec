#include "xmlsigninfo.h"

QXmlSignInfo::QXmlSignInfo() :
  canonicalizationAlgorithm("http://www.w3.org/TR/2001/REC-xml-c14n-20010315"),
  signatureAlgorithm       ("http://www.w3.org/2001/04/xmldsig-more#rsa-sha256")
{
}

QDomElement QXmlSignInfo::generate(QXmlSignContext& context)
{
  QDomElement signedInfo             = context.createElement("SignedInfo");
  QDomElement canonicalizationMethod = context.createElement("CanonicalizationMethod");
  QDomElement signatureMethod        = context.createElement("SignatureMethod");

  canonicalizationMethod.setAttribute("Algorithm", canonicalizationAlgorithm.toString());
  signatureMethod.setAttribute       ("Algorithm", signatureAlgorithm.toString());
  signedInfo.appendChild(canonicalizationMethod);
  signedInfo.appendChild(signatureMethod);
  for (const QXmlSignReference reference : m_references)
    signedInfo.appendChild(reference.generate(context));
  return signedInfo;
}
