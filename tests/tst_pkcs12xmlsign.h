#ifndef TST_PKCS12XMLSIGN_H
#define TST_PKCS12XMLSIGN_H

#include <QObject>
#include <QTest>
#include <xmlsec-qt/xmlsign.h>
#include <xmlsec-qt/xmlsec.h>
#include <QTemporaryFile>

class Pkcs12XmlSignTest : public QObject
{
  Q_OBJECT
  static const unsigned char p12Certificate[];
  static const QString password;
  QTemporaryFile certificateFile;
  QDomDocument xmlDocument;

  bool prepareCertificateFile();
  bool prepareXmlDocument();
private slots:
  void canSignDocument();
};

#endif
