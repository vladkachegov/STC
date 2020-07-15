#ifndef CUSTOMXMLHANDLER_H
#define CUSTOMXMLHANDLER_H
#include <QObject>
#include <QFileDialog>
#include <QDebug>
#include <QXmlStreamReader>
#include <QSqlTableModel>
#include <QThread>
#include "databaseentity.h"

class CustomXMLHandler : public QObject
{
    enum XML_INTERACTION_TYPE
    {
        IMPORT,
        EXPORT
    };

    Q_OBJECT
public:
    explicit CustomXMLHandler(QObject *parent = 0);
    CustomXMLHandler(const QString& dir_path);
    // QThread interface
    bool parse_dir_xml(const QString &dir_path);
    bool parse_single_xml(const QString &file_path,QString &error);
    void export_to_xml();
    QVector<DatabaseEntity> get_parse_results() const;
    void set_dir_path(const QString &value);
    void set_entity_to_export(const DatabaseEntity &value);
protected:
    void run();
private:
    QString error_to_string(QXmlStreamReader::Error error);
    QString dir_path;
    DatabaseEntity entity_to_export;
    QVector<DatabaseEntity> parse_results;
    QStringList bad_xml_files;
    XML_INTERACTION_TYPE interaction_type = XML_INTERACTION_TYPE::IMPORT;
signals:
    void isFinished();
    void file_parsed_signal(const int &file_number, const int &total_files_number,
                            const QString &file_path, const int &isOk, const QString &error_text = "");
    void file_exported_signal();
    void dir_parsed_signal();

public slots:
    void on_thread_start();
    void on_thread_end();
};

#endif // CUSTOMXMLHANDLER_H
