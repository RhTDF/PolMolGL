#include "timelinewidget.h"

#include <QDebug>

TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
    m_NumFrames = 51;
    m_isForward = true;
    m_ForwardPlayBtn = new QPushButton(QIcon(QPixmap(":/icons/play.png")),"");
    m_BackwardPlayBtn = new QPushButton(QIcon(QPixmap(":/icons/replay.png")),"");
    m_PauseBtn = new QPushButton(QIcon(QPixmap(":/icons/pauseon.png")),"");
    m_IncrementFrameBtn = new QPushButton(QIcon(QPixmap(":/icons/skipf.png")),"");
    m_DecrementFrameBtn = new QPushButton(QIcon(QPixmap(":/icons/skipb.png")),"");
    m_FPSInput = new QDoubleSpinBox;
    m_FPSInput->setRange(1,200);
    m_FPSInput->setValue(1);
    QString framecounter = "0/" + QString::number(m_NumFrames-1);
    m_FrameCounter = new QLabel(framecounter);

    m_ForwardPlayCheckBox = new QCheckBox("Forward");
    m_BackwardPlayCheckBox = new QCheckBox("Backwards");
    m_ForwardBackwardPlayCheckBox = new QCheckBox("Forward and Backwards");
    m_LoopPlayCheckBox = new QCheckBox("Loop");
    m_PlayBtnGroup = new QButtonGroup;
    m_PlayGroupBox = new QGroupBox("Playback direction");

    m_PlayBtnGroup->addButton(m_ForwardPlayCheckBox);
    m_PlayBtnGroup->addButton(m_BackwardPlayCheckBox);
    m_PlayBtnGroup->addButton(m_ForwardBackwardPlayCheckBox);
    m_PlayBtnGroup->setExclusive(true);
    QHBoxLayout *playButtonLayout = new QHBoxLayout;
    playButtonLayout->addWidget(m_ForwardPlayCheckBox);
    playButtonLayout->addWidget(m_BackwardPlayCheckBox);
    playButtonLayout->addWidget(m_ForwardBackwardPlayCheckBox);
    playButtonLayout->addWidget(m_LoopPlayCheckBox);
    m_PlayGroupBox->setLayout(playButtonLayout);

    m_TimelineSlider = new QSlider(Qt::Horizontal);
    m_TimelineSlider->setRange(0,m_NumFrames-1);
    m_TimelineSlider->setTickInterval(1);
    m_TimelineSlider->setValue(0);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(m_TimelineSlider);
    topLayout->addWidget(m_FPSInput);

    topLayout->addWidget(m_DecrementFrameBtn);
    topLayout->addWidget(m_ForwardPlayBtn);
    topLayout->addWidget(m_PauseBtn);
    topLayout->addWidget(m_IncrementFrameBtn);
    topLayout->addWidget(m_BackwardPlayBtn);
    topLayout->addWidget(m_FrameCounter);



    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_PlayGroupBox);
    setLayout(mainLayout);
    m_TimerInterval = 1000/m_FPSInput->value();

    m_PlaybackTimer = new QTimer(this);
    //m_PlaybackTimer->start(m_TimerInterval);
    m_isPlayback = false;

    connect(m_ForwardPlayBtn, SIGNAL(clicked()),
            this, SLOT(playClicked()));
    connect(m_BackwardPlayBtn, SIGNAL(clicked()),
            this, SLOT(backClicked()));
    connect(m_PauseBtn, SIGNAL(clicked()),
            this, SLOT(pauseClicked()));
    connect(m_IncrementFrameBtn, SIGNAL(clicked()),
            this, SLOT(skipFClicked()));
    connect(m_DecrementFrameBtn, SIGNAL(clicked()),
            this, SLOT(skipBClicked()));
    connect(m_FPSInput, SIGNAL(valueChanged(double)),
            this, SLOT(intervalChanged(double)));
    connect(m_PlaybackTimer,SIGNAL(timeout()),
            this, SLOT(timerPing()));

    connect(m_TimelineSlider, SIGNAL(valueChanged(int)),
            this, SLOT(updateFrame()));
    connect(m_TimelineSlider, SIGNAL(sliderPressed()),
            this, SLOT(timelinePressed()));
    connect(m_TimelineSlider, SIGNAL(sliderReleased()),
            this, SLOT(timelineReleased()));
    connect(m_TimelineSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(updateFrame()));

}

void TimelineWidget::setNumberOfFrames(int t)
{
    if (t > 0) m_NumFrames = t;
    else m_NumFrames = 0;
}

int TimelineWidget::getNumberOfFrames()
{
    return m_NumFrames;
}

int TimelineWidget::getPlaybackSpeed()
{
    return -1;
}

void TimelineWidget::timerPing()
{
    emit frameChanged(m_TimelineSlider->value());
    m_TimelineSlider->setValue(m_TimelineSlider->value()+1);

//    if (m_ForwardPlayCheckBox->checkState()) {
//        emit skipf();
//        m_TimelineSlider->setValue(m_TimelineSlider->value()+1);
//    } else if (m_BackwardPlayCheckBox->checkState()) {
//        emit skipb();
//        m_TimelineSlider->setValue(m_TimelineSlider->value()-1);
//    } else if (m_ForwardBackwardPlayCheckBox->checkState() && m_isForward) {
//        emit skipf();
//        m_TimelineSlider->setValue(m_TimelineSlider->value()+1);
//    } else if (m_ForwardBackwardPlayCheckBox->checkState() && !m_isForward) {
//        emit skipb();
//        m_TimelineSlider->setValue(m_TimelineSlider->value()-1);
    //    }
}

void TimelineWidget::updateFrame()
{
    m_FrameCounter->setText(QString::number(m_TimelineSlider->value()) + "/" + QString::number(m_NumFrames-1));
    emit frameChanged(m_TimelineSlider->value());
}

void TimelineWidget::updateFrame(int t)
{
    m_FrameCounter->setText(QString::number(t) + "/" + QString::number(m_NumFrames-1));
    emit frameChanged(t);
}

void TimelineWidget::timelinePressed()
{
    m_PlaybackTimer->stop();
}

void TimelineWidget::timelineReleased()
{
    if (m_isPlayback) {
        m_PlaybackTimer->start(m_TimerInterval);
    }
}

void TimelineWidget::lastFrame()
{
    if (!m_LoopPlayCheckBox->checkState()) {
        if (m_ForwardBackwardPlayCheckBox->checkState() && m_isForward) m_isForward = !m_isForward;
        else m_PlaybackTimer->stop();
    } else if (m_ForwardBackwardPlayCheckBox->checkState()) m_isForward = !m_isForward;
    else if (m_ForwardPlayCheckBox->checkState()) m_TimelineSlider->setValue(0);
    else if (m_BackwardPlayCheckBox->checkState()) m_TimelineSlider->setValue(m_TimelineSlider->maximum());

}

void TimelineWidget::playClicked()
{
    m_isPlayback = true;
    m_ForwardPlayBtn->setIcon(QIcon(QPixmap(":/icons/playon.png")));
    m_PauseBtn->setIcon(QIcon(QPixmap(":/icons/pause.png")));
    m_PlaybackTimer->start(m_TimerInterval);
}

void TimelineWidget::backClicked()
{
    m_PlaybackTimer->stop();
    m_TimelineSlider->setValue(0);
    emit frameChanged(0);
}

void TimelineWidget::skipFClicked()
{
    int curFrame = m_TimelineSlider->value();
    if (curFrame < m_TimelineSlider->maximum())
        m_TimelineSlider->setValue(curFrame+1);
}

void TimelineWidget::skipBClicked()
{
    int curFrame = m_TimelineSlider->value();
    if (curFrame > m_TimelineSlider->minimum())
        m_TimelineSlider->setValue(curFrame-1);
}

void TimelineWidget::pauseClicked()
{
    if(m_PlaybackTimer->isActive()) {
        m_PlaybackTimer->stop();
        m_isPlayback = false;
        m_ForwardPlayBtn->setIcon(QIcon(QPixmap(":/icons/play.png")));
        m_PauseBtn->setIcon(QIcon(QPixmap(":/icons/pauseon.png")));
    } else {
        m_PlaybackTimer->start(m_TimerInterval);
    }
}

void TimelineWidget::intervalChanged(double fps)
{
    m_TimerInterval = 1000/fps;
    m_PlaybackTimer->setInterval(m_TimerInterval);
}
