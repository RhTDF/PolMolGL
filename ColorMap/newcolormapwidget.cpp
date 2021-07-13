#include "newcolormapwidget.h"


NewColorMapWidget::NewColorMapWidget(QWidget *parent)
{
    m_NewMapWidgetLayout = new QVBoxLayout;
    m_NewMapComboBox = new QComboBox;
    m_NewMapOkBtn = new QPushButton("Ok");
    m_NewMapNoBtn = new QPushButton("Cancel");

    m_NewMapComboBox->addItem("Sequential");
    m_NewMapComboBox->addItem("Divergent");
    m_NewMapComboBox->addItem("Categorical");
    m_NewMapComboBox->addItem("Rainbow");

    m_NewMapWidgetLayout->addWidget(m_NewMapComboBox);
    m_NewMapWidgetLayout->addWidget(m_NewMapNoBtn);
    m_NewMapWidgetLayout->addWidget(m_NewMapOkBtn);

    setLayout(m_NewMapWidgetLayout);

    connect(m_NewMapNoBtn, SIGNAL(clicked()),
            this, SIGNAL(noPressed()));
    connect(m_NewMapOkBtn, SIGNAL(clicked()),
            this, SLOT(okPressed()));

}

void NewColorMapWidget::okPressed()
{
    emit okPressed(m_NewMapComboBox->currentIndex());
}
