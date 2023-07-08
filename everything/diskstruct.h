#ifndef DISKSTRUCT_H
#define DISKSTRUCT_H

#include "datareader.h"

struct FileInfo {
    //qint64 id;
    QString name;
    QString path;
    QString suffix;
    QString size;
    QString time;
    QString disk;
};
Q_DECLARE_METATYPE(FileInfo)

template<>
inline void MemberField<FileInfo>::init()
{
    //initMember("id",&FileInfo::id,QStringLiteral("ID"));
    initMember("name",&FileInfo::name,QStringLiteral("�ļ���"));
    initMember("path",&FileInfo::path,QStringLiteral("�ļ�·��"));
    initMember("suffix",&FileInfo::suffix,QStringLiteral("��չ��"));
    initMember("size",&FileInfo::size,QStringLiteral("�ļ���С"));
    initMember("time",&FileInfo::time,QStringLiteral("�޸�ʱ��"));
    initMember("disk",&FileInfo::disk,QStringLiteral("���ڴ���"));
}
#endif // DISKSTRUCT_H
