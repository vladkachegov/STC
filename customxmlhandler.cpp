#include "customxmlhandler.h"

CustomXMLHandler::CustomXMLHandler(QObject *parent): QObject(parent)
{

}

CustomXMLHandler::CustomXMLHandler(const QString &dir_path)
{
    this->dir_path = dir_path;
}

void CustomXMLHandler::run()
{
    switch(interaction_type)
    {
    case(XML_INTERACTION_TYPE::IMPORT):
    {
        parse_dir_xml(dir_path);
        emit dir_parsed_signal();
        break;
    }
    case(XML_INTERACTION_TYPE::EXPORT):
    {
        export_to_xml();
        this->interaction_type = XML_INTERACTION_TYPE::IMPORT;
        emit file_exported_signal();
        break;
    }
    }
    emit isFinished();
}

QString CustomXMLHandler::error_to_string(QXmlStreamReader::Error error)
{
    QString error_text;
    switch (error)
    {
    case(QXmlStreamReader::NotWellFormedError):
    {
        error_text = "XML file is not well-formed.";
        break;
    }
    case(QXmlStreamReader::PrematureEndOfDocumentError):
    {
        error_text = "Premature end of document.The input stream ended before a well-formed XML document was parsed.";
        break;
    }
    case(QXmlStreamReader::UnexpectedElementError):
    {
        error_text = "The parser encountered an element that was different to those it expected.";
        break;
    }
    default:
    {
        error_text = "Unexpected of user-defined error!";
        break;
    }

    }
    return error_text;
}

void CustomXMLHandler::set_entity_to_export(const DatabaseEntity &value)
{
    entity_to_export = value;
    this->interaction_type = XML_INTERACTION_TYPE::EXPORT;
}

void CustomXMLHandler::set_dir_path(const QString &value)
{
    dir_path = value;
}

QVector<DatabaseEntity> CustomXMLHandler::get_parse_results() const
{
    return parse_results;
}

void CustomXMLHandler::on_thread_start()
{
    qDebug()<<"XML Thread started";
    this->run();
}

void CustomXMLHandler::on_thread_end()
{
    qDebug()<<"XML Thread ended";
}
bool CustomXMLHandler::parse_single_xml(const QString& file_path, QString &error)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    QXmlStreamReader xml(&file);
    QString entity_data_0;
    QString entity_data_1;
    QString entity_data_2;
    QString entity_data_3;
    QString entity_data_4;
    QString entity_data_5;
    while (!xml.atEnd())
    {

        if (xml.isStartElement() && xml.name() != "root")
        {
            QString element_name = xml.name().toString();

            if (element_name==DatabaseEntity::db_column_names[0])
            {
                entity_data_0 = xml.readElementText();

            }else if (element_name == DatabaseEntity::db_column_names[1])
            {
                entity_data_1 = xml.readElementText();

            }else if(element_name == DatabaseEntity::db_column_names[2])
            {
                entity_data_2 = xml.readElementText();

            }else if(element_name == DatabaseEntity::db_column_names[3])
            {
                entity_data_3 = xml.readElementText();

            }else if(element_name == DatabaseEntity::db_column_names[4])
            {
                entity_data_4 = xml.readElementText();

            }else if(element_name == DatabaseEntity::db_column_names[5])
            {
                entity_data_5 = xml.readElementText();

            }else
            {
                qDebug() << "XML parsing trouble!";
            }

        }
        if (xml.hasError()) // something went wrong while parsing xml file
        {
            error = error_to_string(xml.error());
            file.close();
            return false;
        }
        xml.readNext();
    }
    DatabaseEntity entity(entity_data_0,entity_data_1,entity_data_2,
                          entity_data_3,entity_data_4,entity_data_5);
    parse_results.push_back(entity);
    file.close();
    return true;
}

void CustomXMLHandler::export_to_xml()
{
    QDir dir_to_export = QDir::currentPath();
    if (!QDir("Exported_xmls").exists())
    {
        dir_to_export.mkdir(dir_to_export.path()+"//Exported_xmls");
    }
    dir_to_export.cd("Exported_xmls");

    QString path = dir_to_export.filePath(entity_to_export.get_entity_data()[0] + ".xml");
    QFile new_xml_file(path);
    if (new_xml_file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QXmlStreamWriter xml_writer(&new_xml_file);
        xml_writer.setAutoFormatting(true);
        xml_writer.writeStartDocument();
        xml_writer.writeStartElement("root");// root
        for (int ii = 0; ii<6;++ii)
        {
            QString title;
            QString content;
            title = DatabaseEntity::db_column_names[ii];
            content = entity_to_export.get_entity_data()[ii];
            xml_writer.writeTextElement(title,content);
        }
        xml_writer.writeEndElement();
        xml_writer.writeEndDocument();
    }
    new_xml_file.close();
}



bool CustomXMLHandler::parse_dir_xml(const QString &dir_path)
{
    parse_results.clear();
    QStringList filter("*.xml"); // regexpr
    QDir current_dir(dir_path);
    QFileInfoList fil = current_dir.entryInfoList(filter);
    if (fil.isEmpty())
    {
        return false;
    }else
    {
        int current = 1;
        int total = fil.size();
        Q_FOREACH (const QFileInfo& fi, fil)
        {
            QString error;
            bool good_xml = parse_single_xml(fi.absoluteFilePath(),error);
            if (good_xml)
            {
                emit file_parsed_signal(current,total,fi.fileName(),1);
            }else
            {
                emit file_parsed_signal(current,total,fi.fileName(),0,error);
            }
            ++current;
            //            QThread::sleep(2);
        }
    }
    return true;
}



