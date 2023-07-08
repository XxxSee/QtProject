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
    initMember("name",&FileInfo::name,QStringLiteral("文件名"));
    initMember("path",&FileInfo::path,QStringLiteral("文件路径"));
    initMember("suffix",&FileInfo::suffix,QStringLiteral("扩展名"));
    initMember("size",&FileInfo::size,QStringLiteral("文件大小"));
    initMember("time",&FileInfo::time,QStringLiteral("修改时间"));
    initMember("disk",&FileInfo::disk,QStringLiteral("所在磁盘"));
}
#endif // DISKSTRUCT_H
