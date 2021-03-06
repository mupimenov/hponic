#ifndef WIDGET_PROGRAMS_H
#define WIDGET_PROGRAMS_H

#include <QWidget>
#include <QSplitter>

#include <QSharedPointer>

#include "hponic.h"

namespace Ui {
class WidgetPrograms;
}

class WidgetPrograms : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPrograms(QSharedPointer<Hponic> hponic, QWidget *parent = 0);
    ~WidgetPrograms();

private Q_SLOTS:
    void setUnknownProgramType();
    void setTimerControlType();
    void setRelayControlType();
    void setPidControlType();
    void setButtonControlType();

    void onSetProgramTypeClicked();

    void showContextMenu(QPoint);

    void onProgramCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

    void onProgramsDownloadStarted();
    void onProgramsDownloadFinished(bool success);

    void onProgramsUploadStarted();
    void onProgramsUploadFinished(bool success);

private:
    void createWidgets();
    void createMenu();
    void createLayouts();
    void createConnections();

    void swapWidgetConfigProgram(QWidget *widget);

    Ui::WidgetPrograms *ui;

    QSharedPointer<Hponic> d_hponic;
    QWidget *d_widgetConfigProgram;
    QSplitter *d_splitter;
    QSharedPointer<Program> d_selectedProgram;
};

#endif // WIDGET_PROGRAMS_H
