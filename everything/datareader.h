#ifndef DATAREADER_H
#define DATAREADER_H

#include <QVariant>
#include <QMap>
#include <QSharedPointer>
#include <QDebug>

template <class CT>
class AbstractMember
{
public:
    using SP = QSharedPointer<AbstractMember>;
    virtual void setValue(CT &d, const QVariant &val) = 0;
    virtual void value(const CT &d, QVariant &val) = 0;
};

template <class CT, class MT>
class Member : public AbstractMember<CT>
{
public:
    Member(MT CT::*p):ptr(p){}
    void setValue(CT &d, const QVariant &val) override {
        d.*ptr = val.value<MT>();
    }
    void value(const CT &d, QVariant &val) override {
        val = QVariant::fromValue<MT>(d.*ptr);
    }
private:
    MT CT::*ptr;
};

template <class CT>
class MemberField
{
public:
    MemberField(){ init(); }
    ~MemberField(){m_mMemPtr.clear();}
    template <class MT>
    bool setValue(const QString &name, CT &d, const MT &val) {
        if (m_mMemPtr.contains(name)) {
            m_mMemPtr[name]->setValue(d, QVariant::fromValue<MT>(val));
            return true;
        }
        return false;
    }
    template <class MT>
    bool setValue(const int &index, CT &d, const MT &val) {
        if (m_mMemIndex.contains(index)) {
            if (m_mMemPtr.contains(m_mMemIndex[index])) {
                m_mMemPtr[m_mMemIndex[index]]->setValue(d, QVariant::fromValue<MT>(val));
                return true;
            }
        }
        return false;
    }

    template <class MT>
    bool value(const QString &name, const CT &d, MT &val) const {
        if (m_mMemPtr.contains(name)) {
            QVariant variant;
            m_mMemPtr[name]->value(d, variant);
            val = variant.value<MT>();
            return true;
        }
        return false;
    }
    template <class MT>
    bool value(const int &index, const CT &d, MT &val) const {
        if (m_mMemIndex.contains(index)) {
            if (m_mMemPtr.contains(m_mMemIndex[index])) {
                QVariant variant;
                m_mMemPtr[m_mMemIndex[index]]->value(d, variant);
                val = variant.value<MT>();
                return true;
            }
        }
        return false;
    }

    QString label(const int &index) const {
        if (m_mMemIndex.contains(index)) {
            if (m_mMemLab.contains(m_mMemIndex[index])) {
                return m_mMemLab[m_mMemIndex[index]];
            }
        }
        return QString("");
    }

    int memberCount() const {
        return m_mMemPtr.count();
    }
protected:
    void init();
    template <class MT>
    void initMember(const QString &name, MT CT::*p, const QString &label = "") {
        m_mMemPtr[name] = QSharedPointer<Member<CT,MT>>(new Member<CT,MT>(p));
        m_mMemLab[name] = label;
        m_mMemIndex[m_mMemIndex.size()] = name;
    }
private:
    QMap<QString,typename AbstractMember<CT>::SP> m_mMemPtr;
    QMap<QString,QString> m_mMemLab;
    QMap<int,QString> m_mMemIndex;
};

#endif // DATAREADER_H
