#ifndef  XMLSECQTSECKEY_H
# define XMLSECQTSECKEY_H

# include "xmlsec-qt_global.h"
# include <xmlsec/xmlsec.h>
# include <QByteArray>

class QSslKey;

xmlSecKeyPtr qSslKeyToXmlSecKey(const QSslKey&, const QByteArray& = QByteArray());

#endif
