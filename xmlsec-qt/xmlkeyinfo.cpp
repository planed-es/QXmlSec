#include "xmlkeyinfo.h"
/*
#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/templates.h>
#include <xmlsec/crypto.h>
*/

QDomElement QXmlSignKeyInfo::generate(QXmlSignContext& context)
{
  QDomElement keyInfo  = context.createElement("KeyInfo");

  if (id.length() > 0)
    keyInfo.setAttribute("Id", id);
  if (keyNameEnabled)
    keyInfo.appendChild(context.createElement("KeyName"));
  if (keyValueEnabled)
    keyInfo.appendChild(context.createElement("KeyValue"));
  if (x509dataEnabled)
    keyInfo.appendChild(context.createElement("X509Data"));
  return keyInfo;
}
