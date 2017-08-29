#include "settingadeverimgwidget.h"
#include "ui_settingadeverimgwidget.h"

SettingAdeverImgWidget::SettingAdeverImgWidget(QWidget *parent) :
    QWidget(0, Qt::FramelessWindowHint),
    ui(new Ui::SettingAdeverImgWidget)
{
    setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);
    initConnect();
}

SettingAdeverImgWidget::~SettingAdeverImgWidget()
{
    delete ui;
    qDebug("delete SettingAdeverImgWidget...");
}

/**
 * @brief SettingAdeverImgWidget::setCurrentAdverImgSize
 * @param w
 * @param h
 */
void SettingAdeverImgWidget::setCurrentAdverImgSize(int w, int h)
{
    QString adverImgeWidth;
    QString adverImgHeight;

    /*get default advertisement image size*/
    adverImgeWidth = adverImgeWidth.setNum(w);
    adverImgHeight = adverImgHeight.setNum(h);

    ui->widthVal->setText(adverImgeWidth);
    ui->heightVal->setText(adverImgHeight);
}

/**
 * @brief SettingAdeverImgWidget::slotApplySettingVal
 */
void SettingAdeverImgWidget::slotApplySettingVal()
{
    emit sendSettingVal(ui->widthVal->text().toInt(), ui->heightVal->text().toInt());
    emit sendAdverUrlInfo(ui->URL->text());

    this->close();
}

/**
 * @brief SettingAdeverImgWidget::slotCancelSetting
 */
void SettingAdeverImgWidget::slotCancelSetting()
{
    this->close();
}

/**
 * @brief SettingAdeverImgWidget::initConnect
 */
void SettingAdeverImgWidget::initConnect()
{
    connect(ui->applySetting  ,SIGNAL(clicked()),SLOT(slotApplySettingVal()));
    connect(ui->cancelSetting ,SIGNAL(clicked()),SLOT(slotCancelSetting()));
}
