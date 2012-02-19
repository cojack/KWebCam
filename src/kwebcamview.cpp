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

KWebCamView::KWebCamView(QWidget *) : surface(0)
     , playButton(0)
     , positionSlider(0)
{
     connect(&movie, SIGNAL(stateChanged(QMovie::MovieState)),
             this, SLOT(movieStateChanged(QMovie::MovieState)));
     connect(&movie, SIGNAL(frameChanged(int)),
             this, SLOT(frameChanged(int)));  
  
  
    ui_kwebcamview_base.setupUi(this);
    settingsChanged();
    setAutoFillBackground(true);    
}

KWebCamView::~KWebCamView()
{

}

void KWebCamView::runVideo(QString device)
{
    //Phonon::MediaObject *mediaObject = new Phonon::MediaObject(this);
    //Phonon::VideoWidget *videoWidget = new Phonon::VideoWidget(this);
    //mediaObject->setCurrentSource(device);
    //Phonon::createPath(mediaObject, this->ui_kwebcamview_base.videoPlayer);

    //Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory, this);
    //Phonon::createPath(mediaObject, audioOutput);
    this->ui_kwebcamview_base.videoPlayer->play(device);
    kDebug() << device;
    //QLabel myLabel;
    //myLabel.setPixmap(QPixmap::fromImage(this->ui_kwebcamview_base.videoPlayer.videoWidget->snapshot()));
    //myLabel.show();     
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
