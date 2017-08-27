/*
 * main.cpp
 * This file is part of FoscamCGIInterface
 *
 * Copyright (C) 2017-2017 Ludovic Girardet
 *
 * FoscamCGIInterface is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FoscamCGIInterface is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    QApplication a(argc, argv);
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit ()));

    a.setApplicationName("Foscam CGI Interface");
    a.setApplicationVersion(VERSION_STRING);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.process(a);

    MainWindow w;
    w.show();

    return a.exec();
}
