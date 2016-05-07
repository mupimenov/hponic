#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QDateTime>

#include <QSharedPointer>

enum ProgramType
{
    EmptyProgramType,

    TimerControlType,
    RelayControlType,
    PidControlType,
    ButtonControlType
};

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(int id, int type, QObject *parent = 0);
    virtual ~Program();

    void setName(const QString &name);
    const QString &name() const;

    int id() const;
    int type() const;

Q_SIGNALS:
    void changed(Program *sender = 0);

public Q_SLOTS:

protected:
    int d_id;
    int d_type;
    QString d_name;
};

/*
 * EmptyProgram
 * */

class EmptyProgram : public Program
{
    Q_OBJECT
public:
    explicit EmptyProgram(int id, QObject *parent = 0);
    virtual ~EmptyProgram();

Q_SIGNALS:

public Q_SLOTS:

};

struct Cyclogram
{
    enum Type {
        Simple,
        Impulse
    };

    Type type;
    int count;
    int impulseDuration;
    int pauseDuration;

    Cyclogram();
    virtual ~Cyclogram();
};

enum TimeConstraints
{
    AllTime = 0,
    StrictEquality,
    EveryDay,
    EveryMonth,
    EveryYear
};

/*
 * TimerControlProgram
 * */

class TimerControlProgram : public Program
{
    Q_OBJECT
public:
    explicit TimerControlProgram(int id, QObject *parent = 0);
    virtual ~TimerControlProgram();

    void setConstrains(int constrains);
    int constrains(void) const;

    void setFrom(const QDateTime &from);
    const QDateTime &from() const;

    void setTo(const QDateTime &to);
    const QDateTime &to() const;

    void setCyclogram(const Cyclogram &cyclogram);
    const Cyclogram &cyclogram() const;

    void setOutput(int id);
    int output() const;

Q_SIGNALS:

public Q_SLOTS:

private:    
    int d_constrains;
    QDateTime d_from;
    QDateTime d_to;
    Cyclogram d_cyclogram;
    int d_output;
};

/*
 * RelayControlProgram
 * */

class RelayControlProgram : public Program
{
    Q_OBJECT
public:
    explicit RelayControlProgram(int id, QObject *parent = 0);
    virtual ~RelayControlProgram();

    void setInput(int id);
    int input() const;

    void setConstrains(int constrains);
    int constrains(void) const;

    void setFrom(const QDateTime &from);
    const QDateTime &from() const;

    void setTo(const QDateTime &to);
    const QDateTime &to() const;

    void setLowBound(float lowBound);
    float lowBound() const;

    void setHighBound(float highBound);
    float highBound() const;

    void setCyclogram(const Cyclogram &cyclogram);
    const Cyclogram &cyclogram() const;

    void setInverse(bool inverse);
    bool inverse() const;

    void setOutput(int id);
    int output() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_input;

    int d_constrains;
    QDateTime d_from;
    QDateTime d_to;

    float d_lowBound;
    float d_highBound;

    Cyclogram d_cyclogram;

    bool d_inverse;

    int d_output;
};

/*
 * PidControlProgram
 * */

class PidControlProgram : public Program
{
    Q_OBJECT
public:
    explicit PidControlProgram(int id, QObject *parent = 0);
    virtual ~PidControlProgram();

    void setInput(int id);
    int input() const;

    void setConstrains(int constrains);
    int constrains(void) const;

    void setFrom(const QDateTime &from);
    const QDateTime &from() const;

    void setTo(const QDateTime &to);
    const QDateTime &to() const;

    void setDesired(float desired);
    float desired() const;

    void setProportional(float p);
    float proportional() const;

    void setIntegral(float i);
    float integral() const;

    void setDifferential(float d);
    float differential() const;

    void setInverse(bool inverse);
    bool inverse() const;

    void setOutput(int id);
    int output() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_input;

    int d_constrains;
    QDateTime d_from;
    QDateTime d_to;

    float d_desired;

    float d_proportional;
    float d_integral;
    float d_differential;

    bool d_inverse;

    int d_output;
};

/*
 * ButtonControlProgram
 * */

class ButtonControlProgram : public Program
{
    Q_OBJECT
public:
    explicit ButtonControlProgram(int id, QObject *parent = 0);
    virtual ~ButtonControlProgram();

    void setInput(int id);
    int input() const;

    void setOutput(int id);
    int output() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_input;

    int d_output;
};

/*
 * ProgramConv
 * */

class ProgramConv
{
public:
    template<class P>
    static QSharedPointer<P> toProgram(QSharedPointer<Program> &program) {
        QSharedPointer<P> p = program.dynamicCast<P>();
        return p;
    }
};

#endif // PROGRAM_H
