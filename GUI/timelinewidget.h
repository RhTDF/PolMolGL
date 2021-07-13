#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QButtonGroup>
#include <QGroupBox>
#include <QSlider>
#include <QIcon>
#include <QPixmap>
#include <QDoubleSpinBox>
#include <QLabel>

class TimelineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    void setNumberOfFrames(int t);
    int getNumberOfFrames();
    int getPlaybackSpeed();
signals:
    void frameChanged(int t);
    void play();
    void back();
    void skipf();
    void skipb();
    void pause();
public slots:
    void timerPing();
    void updateFrame();
    void updateFrame(int t);
    void timelinePressed();
    void timelineReleased();

    void lastFrame();
    void playClicked();
    void backClicked();
    void skipFClicked();
    void skipBClicked();
    void pauseClicked();
    void intervalChanged(double fps);
private:
    QPushButton *m_ForwardPlayBtn;
    QPushButton *m_BackwardPlayBtn;
    QPushButton *m_PauseBtn;
    QPushButton *m_IncrementFrameBtn;
    QPushButton *m_DecrementFrameBtn;
    QTimer *m_PlaybackTimer;
    QDoubleSpinBox *m_FPSInput;
    QCheckBox *m_ForwardPlayCheckBox;
    QCheckBox *m_BackwardPlayCheckBox;
    QCheckBox *m_LoopPlayCheckBox;
    QCheckBox *m_ForwardBackwardPlayCheckBox;
    QButtonGroup *m_PlayBtnGroup;
    QGroupBox *m_PlayGroupBox;
    QSlider *m_TimelineSlider;
    QLabel *m_FrameCounter;
    int m_TimerInterval;
    bool m_isForward;
    bool m_isPlayback;
    int m_NumFrames;
};

#endif // TIMELINEWIDGET_H
