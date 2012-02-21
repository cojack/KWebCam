/***************************************************************************
 *   Copyright (C) 2012 by Przemysław Czekaj <xcojack@gmail.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "kwebcamview.h"
#include "settings.h"

#include <KLocale>
#include <QtGui/QLabel>

#include <Phonon/MediaObject>
#include <Phonon/VideoWidget>
#include <Phonon/AudioOutput>

#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QFileDialog>

KWebCamView::KWebCamView(QWidget *) : surface(0)
     , playButton(0)
     , positionSlider(0)
{
     connect(&movie, SIGNAL(stateChanged(QMovie::MovieState)),
             this, SLOT(movieStateChanged(QMovie::MovieState)));
     connect(&movie, SIGNAL(frameChanged(int)),
             this, SLOT(frameChanged(int)));  
  
  
    ui_kwebcamview_base.setupUi(this);
    
    ui_kwebcamview_base.screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui_kwebcamview_base.screenshotLabel->setAlignment(Qt::AlignCenter);
    ui_kwebcamview_base.screenshotLabel->setMinimumSize(240, 160);    
    
    settingsChanged();
    setAutoFillBackground(true);    
}

KWebCamView::~KWebCamView()
{

}

void KWebCamView::runVideo(QString device)
{
    Phonon::MediaObject *mediaObject = new Phonon::MediaObject(this);

    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(this);    
    Phonon::VideoWidget *videoWidget = this->ui_kwebcamview_base.VideoWidget;
//.videoPlayer->videoWidget();
    
    connect(ui_kwebcamview_base.playButton, SIGNAL(clicked()), mediaObject, SLOT(play()));
    connect(ui_kwebcamview_base.stopButton, SIGNAL(clicked()), mediaObject, SLOT(stop()));
    
    
    Phonon::Path audioPath = Phonon::createPath(mediaObject, audioOutput);
    Phonon::Path videoPath = Phonon::createPath(mediaObject, videoWidget);    
    
    if (!audioPath.isValid()) {
        QMessageBox::critical(this, "Error", "Your backend may not support audio capturing.");
    }
    if (!videoPath.isValid()) {
        QMessageBox::critical(this, "Error", "Your backend may not support video capturing.");
    }
    
    
    Phonon::MediaSource source(Phonon::Capture::VideoType, Phonon::NoCaptureCategory);
    kDebug() << source.type();
    mediaObject->setCurrentSource(source);    
    
    connect(this->ui_kwebcamview_base.brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
    connect(this->ui_kwebcamview_base.contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(setContrast(int)));
    connect(this->ui_kwebcamview_base.hueSlider, SIGNAL(valueChanged(int)), this, SLOT(setHue(int)));
    connect(this->ui_kwebcamview_base.saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(setSaturation(int)));
    connect(this->ui_kwebcamview_base.snapShotButton, SIGNAL(clicked()), this, SLOT(takeSnapShot()));
    
    
    kDebug() << QString("Gdzie ja kurwa jestem? ") + device;
    
    //mediaObject->play();
    //this->ui_kwebcamview_base.videoPlayer->play();

    //QLabel myLabel;
    //myLabel.setPixmap(QPixmap::fromImage(this->ui_kwebcamview_base.videoPlayer.videoWidget->snapshot()));
    //myLabel.show();     
}

void KWebCamView::setBrightness(int i)
{
  this->ui_kwebcamview_base.VideoWidget->setBrightness((qreal)i/100);
}

void KWebCamView::setContrast(int i)
{
  this->ui_kwebcamview_base.VideoWidget->setContrast((qreal)i/100);
}

void KWebCamView::setHue(int i)
{
  this->ui_kwebcamview_base.VideoWidget->setHue((qreal)i/100);
}

void KWebCamView::setSaturation(int i)
{
  this->ui_kwebcamview_base.VideoWidget->setSaturation((qreal)i/100);
}

void KWebCamView::takeSnapShot()
{
  //QImage originalImage = this->ui_kwebcamview_base.VideoWidget->snapshot();
  
  QPixmap originalImage = QPixmap::grabWidget(this->ui_kwebcamview_base.VideoWidget);
  
  QString format = "png";
  QString initialPath = QDir::currentPath() + tr("/dupa.") + format;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath, tr("%1 Files (*.%2);;All Files (*)").arg(format.toUpper()).arg(format));
  
  if(originalImage.isNull()) {
    QMessageBox::critical(this, "Error", "Obrazek jest pusty kurwa.");
    return;
  }
  
  if (!fileName.isEmpty())
      kDebug() << originalImage.save(fileName, "PNG");  
  
  //ui_kwebcamview_base.screenshotLabel->setPixmap(originalPixmap.scaled(ui_kwebcamview_base.screenshotLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void KWebCamView::switchColors()
{
    // switch the foreground/background colors of the label
    QColor color = Settings::col_background();
    Settings::setCol_background( Settings::col_foreground() );
    Settings::setCol_foreground( color );

    settingsChanged();
}

void KWebCamView::settingsChanged()
{
    QPalette pal;
    pal.setColor( QPalette::Window, Settings::col_background());
    pal.setColor( QPalette::WindowText, Settings::col_foreground());
    //ui_kwebcamview_base.kcfg_sillyLabel->setPalette( pal );

    // i18n : internationalization
    //ui_kwebcamview_base.kcfg_sillyLabel->setText( i18n("This project is %1 days old",Settings::val_time()) );
    emit signalChangeStatusbar( i18n("Settings changed") );
}

#include "kwebcamview.moc"
