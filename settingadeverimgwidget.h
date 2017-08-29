#ifndef SETTINGADEVERIMGWIDGET_H
#define SETTINGADEVERIMGWIDGET_H

#include <QWidget>

namespace Ui {
class SettingAdeverImgWidget;
}

class SettingAdeverImgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingAdeverImgWidget(QWidget *parent = 0);
    ~SettingAdeverImgWidget();

    void setCurrentAdverImgSize(int w, int h);

private:
    Ui::SettingAdeverImgWidget *ui;
    void initConnect();

private slots:
    void slotApplySettingVal();
    void slotCancelSetting();

signals:
    void sendSettingVal( int w, int y );
    void sendAdverUrlInfo( QString url );
};

#endif // SETTINGADEVERIMGWIDGET_H
