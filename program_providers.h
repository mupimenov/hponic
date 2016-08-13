#ifndef PROGRAMPROVIDERS_H
#define PROGRAMPROVIDERS_H

#include <QByteArray>
#include <QSharedPointer>
#include <QList>

#include <QWidget>

#include <QDomDocument>
#include <QDomElement>

extern void floatToArray(float f, QByteArray &arr, int offset);
extern float arrayToFloat(const QByteArray &arr, int offset);

class Program;
class Hponic;

class EmptyProgram;
class TimerControlProgram;
class RelayControlProgram;
class PidControlProgram;
class ButtonControlProgram;

/* Basic */

class ProgramEditorProvider
{
public:
    virtual ~ProgramEditorProvider() {}

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0) = 0;

};

class ProgramBinProvider
{
public:
    virtual ~ProgramBinProvider() {}

    virtual QByteArray toArray() = 0;
    virtual void fromArray(const QByteArray &arr) = 0;

};

class ProgramXmlProvider
{
public:
    virtual ~ProgramXmlProvider() {}

    virtual void toElement(QDomElement &elem) = 0;
    virtual void fromElement(QDomElement &elem) = 0;

};

/* EmptyProgram */

class EmptyProgramEditorProvider : public ProgramEditorProvider
{
public:
    explicit EmptyProgramEditorProvider(QSharedPointer<EmptyProgram> program);
    virtual ~EmptyProgramEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<EmptyProgram> d_program;
};

class EmptyProgramBinProvider : public ProgramBinProvider
{
public:
    explicit EmptyProgramBinProvider(QSharedPointer<EmptyProgram> program);
    virtual ~EmptyProgramBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<EmptyProgram> d_program;
};

class EmptyProgramXmlProvider : public ProgramXmlProvider
{
public:
    explicit EmptyProgramXmlProvider(QSharedPointer<EmptyProgram> program);
    virtual ~EmptyProgramXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<EmptyProgram> d_program;
};

/* TimerControlProgram */

class TimerControlProgramEditorProvider : public ProgramEditorProvider
{
public:
    explicit TimerControlProgramEditorProvider(QSharedPointer<TimerControlProgram> program);
    virtual ~TimerControlProgramEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<TimerControlProgram> d_program;
};

class TimerControlProgramBinProvider : public ProgramBinProvider
{
public:
    explicit TimerControlProgramBinProvider(QSharedPointer<TimerControlProgram> program);
    virtual ~TimerControlProgramBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<TimerControlProgram> d_program;
};

class TimerControlProgramXmlProvider : public ProgramXmlProvider
{
public:
    explicit TimerControlProgramXmlProvider(QSharedPointer<TimerControlProgram> program);
    virtual ~TimerControlProgramXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<TimerControlProgram> d_program;
};

/* RelayControlProgram */

class RelayControlProgramEditorProvider : public ProgramEditorProvider
{
public:
    explicit RelayControlProgramEditorProvider(QSharedPointer<RelayControlProgram> program);
    virtual ~RelayControlProgramEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<RelayControlProgram> d_program;
};

class RelayControlProgramBinProvider : public ProgramBinProvider
{
public:
    explicit RelayControlProgramBinProvider(QSharedPointer<RelayControlProgram> program);
    virtual ~RelayControlProgramBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<RelayControlProgram> d_program;
};

class RelayControlProgramXmlProvider : public ProgramXmlProvider
{
public:
    explicit RelayControlProgramXmlProvider(QSharedPointer<RelayControlProgram> program);
    virtual ~RelayControlProgramXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<RelayControlProgram> d_program;
};

/* PidControlProgram */

class PidControlProgramEditorProvider : public ProgramEditorProvider
{
public:
    explicit PidControlProgramEditorProvider(QSharedPointer<PidControlProgram> program);
    virtual ~PidControlProgramEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<PidControlProgram> d_program;
};

class PidControlProgramBinProvider : public ProgramBinProvider
{
public:
    explicit PidControlProgramBinProvider(QSharedPointer<PidControlProgram> program);
    virtual ~PidControlProgramBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<PidControlProgram> d_program;
};

class PidControlProgramXmlProvider : public ProgramXmlProvider
{
public:
    explicit PidControlProgramXmlProvider(QSharedPointer<PidControlProgram> program);
    virtual ~PidControlProgramXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<PidControlProgram> d_program;
};

/* ButtonControlProgram */

class ButtonControlProgramEditorProvider : public ProgramEditorProvider
{
public:
    explicit ButtonControlProgramEditorProvider(QSharedPointer<ButtonControlProgram> program);
    virtual ~ButtonControlProgramEditorProvider();

    virtual QWidget *createEditor(QSharedPointer<Hponic> hponic, QWidget *parent = 0);

private:
    QSharedPointer<ButtonControlProgram> d_program;
};

class ButtonControlProgramBinProvider : public ProgramBinProvider
{
public:
    explicit ButtonControlProgramBinProvider(QSharedPointer<ButtonControlProgram> program);
    virtual ~ButtonControlProgramBinProvider();

    virtual QByteArray toArray();
    virtual void fromArray(const QByteArray &arr);

private:
    QSharedPointer<ButtonControlProgram> d_program;
};

class ButtonControlProgramXmlProvider : public ProgramXmlProvider
{
public:
    explicit ButtonControlProgramXmlProvider(QSharedPointer<ButtonControlProgram> program);
    virtual ~ButtonControlProgramXmlProvider();

    virtual void toElement(QDomElement &elem);
    virtual void fromElement(QDomElement &elem);

private:
    QSharedPointer<ButtonControlProgram> d_program;
};

#endif // PROGRAMPROVIDERS_H
