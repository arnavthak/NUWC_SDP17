#include "pdfreportgenerator.h"

#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QMarginsF>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QColor>
#include <QFont>
#include <QRect>
#include <QStringList>

PdfReportGenerator::PdfReportGenerator(QObject *parent)
    : QObject(parent)
{
}

bool PdfReportGenerator::generateBasicReport(const QString &filePath,
                                             const QString &chipName,
                                             const QString &testMode,
                                             int totalTests,
                                             int passedTests,
                                             int failedTests,
                                             const QVariantList &pinConfigsVar,
                                             const QVariantList &testsVar,
                                             const QStringList &messages)
{
    if (filePath.trimmed().isEmpty()) {
        emit reportFailed("File path is empty.");
        return false;
    }

    QList<PinConfig> pinConfigs;

    for (const QVariant &var : pinConfigsVar) {
        QVariantMap map = var.toMap();

        PinConfig p;
        p.pinName = map["pinName"].toString();
        p.direction = map["direction"].toString();
        p.defaultValue = map["defaultValue"].toString();

        pinConfigs.append(p);
    }

    QList<TestResult> tests;

    for (const QVariant &var : testsVar) {
        QVariantMap map = var.toMap();

        TestResult t;
        t.testName = map["testName"].toString();
        t.responseBytes = map["responseBytes"].toString();
        t.expectedBytes = map["expectedBytes"].toString();
        t.passed = map["passed"].toBool();
        t.durationMs = map["durationMs"].toDouble();

        // inputPins (nested map)
        QVariantMap inputsMap = map["inputPins"].toMap();
        for (auto it = inputsMap.begin(); it != inputsMap.end(); ++it) {
            t.inputPins[it.key()] = it.value().toString();
        }

        tests.append(t);
    }

    QFileInfo fileInfo(filePath);
    QDir folder = fileInfo.dir();

    if (!folder.exists()) {
        emit reportFailed("Folder does not exist: " + folder.absolutePath());
        return false;
    }

    // Filter out placeholder/demo text so it does not appear in the final PDF
    QStringList cleanedMessages;
    for (const QString &msg : messages) {
        const QString trimmed = msg.trimmed();

        if (trimmed.isEmpty())
            continue;

        if (trimmed == "Basic demo report generated from the Results panel.")
            continue;

        if (trimmed == "Replace this placeholder data with real test output later.")
            continue;

        if (trimmed == "Prepared for Friday's meeting demo.")
            continue;

        cleanedMessages.append(trimmed);
    }

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(18, 18, 18, 18));
    writer.setResolution(120);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        emit reportFailed("Could not start PDF writer for: " + filePath);
        return false;
    }

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    const int pageWidth = writer.width();
    const int pageHeight = writer.height();

    const int leftMargin = 100;
    const int rightMargin = 100;
    const int contentWidth = pageWidth - leftMargin - rightMargin;

    int y = 100;

    auto drawRoundedBox = [&](const QRect &rect, const QColor &fill, const QColor &border) {
        painter.save();
        painter.setPen(QPen(border, 2));
        painter.setBrush(fill);
        painter.drawRoundedRect(rect, 16, 16);
        painter.restore();
    };

    auto drawTextBlock = [&](const QRect &rect,
                             const QString &text,
                             const QFont &font,
                             const QColor &color,
                             int flags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap) {
        painter.save();
        painter.setFont(font);
        painter.setPen(color);
        painter.drawText(rect, flags, text);
        painter.restore();
    };

    QFont titleFont("Arial", 22, QFont::Bold);
    QFont subtitleFont("Arial", 10, QFont::Normal);
    QFont sectionTitleFont("Arial", 13, QFont::Bold);
    QFont cardLabelFont("Arial", 10, QFont::Normal);
    QFont cardValueFont("Arial", 18, QFont::Bold);
    QFont bodyFont("Arial", 10, QFont::Normal);
    QFont smallFont("Arial", 9, QFont::Normal);

    const QColor navyText("#0f172a");
    const QColor mutedText("#475569");
    const QColor lightBorder("#dbe3ee");
    const QColor softPanel("#f8fafc");
    const QColor blueFill("#eef6ff");
    const QColor blueBorder("#c7defc");
    const QColor greenFill("#dcfce7");
    const QColor greenText("#15803d");
    const QColor redFill("#fee2e2");
    const QColor redText("#dc2626");
    const QColor purpleFill("#f3e8ff");
    const QColor purpleText("#7e22ce");
    const QColor grayFill("#f8fafc");
    const QColor grayText("#334155");

    // ============================================================
    // HEADER
    // ============================================================
    QRect headerRect(leftMargin, y, contentWidth, 140);
    drawRoundedBox(headerRect, blueFill, blueBorder);

    drawTextBlock(QRect(leftMargin + 28, y + 22, contentWidth - 56, 40),
                  "Circuit Test Report",
                  titleFont,
                  navyText,
                  Qt::AlignLeft | Qt::AlignVCenter);

    drawTextBlock(QRect(leftMargin + 28, y + 68, contentWidth - 56, 22),
                  "Generated from the Circuit GUI test results panel",
                  subtitleFont,
                  mutedText,
                  Qt::AlignLeft | Qt::AlignVCenter);

    drawTextBlock(QRect(leftMargin + 28, y + 96, contentWidth - 56, 24),
                  "Created: " + QDateTime::currentDateTime().toString("MMMM d, yyyy - h:mm AP"),
                  smallFont,
                  mutedText,
                  Qt::AlignLeft | Qt::AlignVCenter);

    y += 170;

    // ============================================================
    // SUMMARY SECTION TITLE
    // ============================================================
    drawTextBlock(QRect(leftMargin, y, contentWidth, 28),
                  "Summary",
                  sectionTitleFont,
                  navyText);

    y += 40;

    // ============================================================
    // SUMMARY CARDS
    // ============================================================
    const int cardSpacing = 20;
    const int cardWidth = (contentWidth - (3 * cardSpacing)) / 4;
    const int cardHeight = 100;

    struct SummaryCard {
        QString title;
        QString value;
        QColor fill;
        QColor valueColor;
    };

    const QString successRate =
        totalTests > 0
            ? QString::number(qRound((passedTests * 100.0) / totalTests)) + "%"
            : "-";

    QList<SummaryCard> cards = {
        { "Total Tests", QString::number(totalTests), grayFill, grayText },
        { "Passed", QString::number(passedTests), greenFill, greenText },
        { "Failed", QString::number(failedTests), redFill, redText },
        { "Success Rate", successRate, purpleFill, purpleText }
    };

    for (int i = 0; i < cards.size(); ++i) {
        const int x = leftMargin + i * (cardWidth + cardSpacing);
        const QRect cardRect(x, y, cardWidth, cardHeight);
        drawRoundedBox(cardRect, cards[i].fill, lightBorder);

        drawTextBlock(QRect(x + 16, y + 16, cardWidth - 32, 20),
                      cards[i].title,
                      cardLabelFont,
                      mutedText);

        drawTextBlock(QRect(x + 16, y + 44, cardWidth - 32, 34),
                      cards[i].value,
                      cardValueFont,
                      cards[i].valueColor);
    }

    y += 130;

    // ============================================================
    // TEST DETAILS SECTION
    // ============================================================
    drawTextBlock(QRect(leftMargin, y, contentWidth, 28),
                  "Test Details",
                  sectionTitleFont,
                  navyText);

    y += 40;

    QRect detailsRect(leftMargin, y, contentWidth, 120);
    drawRoundedBox(detailsRect, softPanel, lightBorder);

    const int detailsLeft = leftMargin + 20;
    const int detailsTop = y + 18;
    const int labelWidth = 120;
    const int valueX = detailsLeft + labelWidth;
    const int lineGap = 26;

    drawTextBlock(QRect(detailsLeft, detailsTop, labelWidth, 20),
                  "Chip:",
                  bodyFont,
                  mutedText);

    drawTextBlock(QRect(valueX, detailsTop, contentWidth - 170, 20),
                  chipName.trimmed().isEmpty() ? "N/A" : chipName,
                  bodyFont,
                  navyText);

    drawTextBlock(QRect(detailsLeft, detailsTop + lineGap, labelWidth, 20),
                  "Mode:",
                  bodyFont,
                  mutedText);

    drawTextBlock(QRect(valueX, detailsTop + lineGap, contentWidth - 170, 20),
                  testMode.trimmed().isEmpty() ? "N/A" : testMode,
                  bodyFont,
                  navyText);

    drawTextBlock(QRect(detailsLeft, detailsTop + 2 * lineGap, labelWidth, 20),
                  "Status:",
                  bodyFont,
                  mutedText);

    QString overallStatus;
    QColor overallStatusColor;

    if (totalTests == 0) {
        overallStatus = "No test data";
        overallStatusColor = mutedText;
    } else if (failedTests == 0) {
        overallStatus = "All tests passed";
        overallStatusColor = greenText;
    } else {
        overallStatus = "Some tests failed";
        overallStatusColor = redText;
    }

    drawTextBlock(QRect(valueX, detailsTop + 2 * lineGap, contentWidth - 170, 20),
                  overallStatus,
                  bodyFont,
                  overallStatusColor);

    y += 150;

    // ============================================================
    // PIN CONFIGURATION TABLE
    // ============================================================
    drawTextBlock(QRect(leftMargin, y, contentWidth, 28),
                  "Pin Configuration",
                  sectionTitleFont,
                  navyText);

    y += 40;

    int tableRowHeight = 28;
    int tableHeight = tableRowHeight * (pinConfigs.size() + 1);

    QRect tableRect(leftMargin, y, contentWidth, tableHeight);
    drawRoundedBox(tableRect, QColor("#ffffff"), lightBorder);

    // Column widths
    int col1 = contentWidth * 0.3;
    int col2 = contentWidth * 0.3;
    int col3 = contentWidth * 0.4;

    // Header row
    drawTextBlock(QRect(leftMargin + 10, y, col1, tableRowHeight),
                  "Pin", bodyFont, navyText);
    drawTextBlock(QRect(leftMargin + col1, y, col2, tableRowHeight),
                  "Direction", bodyFont, navyText);
    drawTextBlock(QRect(leftMargin + col1 + col2, y, col3, tableRowHeight),
                  "Default", bodyFont, navyText);

    // Rows
    int rowY = y + tableRowHeight;

    for (const auto &pin : pinConfigs) {
        drawTextBlock(QRect(leftMargin + 10, rowY, col1, tableRowHeight),
                      pin.pinName, bodyFont, mutedText);

        drawTextBlock(QRect(leftMargin + col1, rowY, col2, tableRowHeight),
                      pin.direction, bodyFont, mutedText);

        drawTextBlock(QRect(leftMargin + col1 + col2, rowY, col3, tableRowHeight),
                      pin.defaultValue, bodyFont, mutedText);

        rowY += tableRowHeight;
    }

    y += tableHeight + 40;

    // ============================================================
    // TEST RESULT CARDS
    // ============================================================
    const int headerHeight = 40;

    // Ensure header fits BEFORE drawing it
    if (y + headerHeight > pageHeight - 120) {
        writer.newPage();
        y = 100;
    }

    drawTextBlock(QRect(leftMargin, y, contentWidth, 28),
                  "Test Results",
                  sectionTitleFont,
                  navyText);

    y += headerHeight;

    for (int i = 0; i < tests.size(); ++i) {

        const auto &test = tests[i];

        int boxHeight = 160 + test.inputPins.size() * 22;

        // =====================================================
        // ✅ PAGE BREAK CHECK (BEFORE DRAWING)
        // =====================================================
        if (y + boxHeight > pageHeight - 120) {
            writer.newPage();
            y = 100;

            // OPTIONAL: redraw section header on new page
            drawTextBlock(QRect(leftMargin, y, contentWidth, 28),
                          "Test Results (cont.)",
                          sectionTitleFont,
                          navyText);
            y += 40;
        }

        QRect boxRect(leftMargin, y, contentWidth, boxHeight);

        QColor fill = test.passed ? greenFill : redFill;
        drawRoundedBox(boxRect, fill, lightBorder);

        int innerX = leftMargin + 20;
        int innerY = y + 15;

        // Test Name
        drawTextBlock(QRect(innerX, innerY, contentWidth - 40, 24),
                      test.testName,
                      sectionTitleFont,
                      navyText);

        innerY += 28;

        // Inputs
        drawTextBlock(QRect(innerX, innerY, 200, 20),
                      "Inputs:",
                      bodyFont,
                      mutedText);

        innerY += 20;

        for (auto it = test.inputPins.begin(); it != test.inputPins.end(); ++it) {
            drawTextBlock(QRect(innerX + 10, innerY, contentWidth - 60, 20),
                          it.key() + " = " + it.value(),
                          smallFont,
                          navyText);
            innerY += 20;
        }

        innerY += 6;

        // Response
        drawTextBlock(QRect(innerX, innerY, contentWidth - 40, 20),
                      "Response: " + test.responseBytes,
                      bodyFont,
                      navyText);
        innerY += 22;

        drawTextBlock(QRect(innerX, innerY, contentWidth - 40, 20),
                      "Expected: " + test.expectedBytes,
                      bodyFont,
                      navyText);
        innerY += 22;

        QString statusText = test.passed ? "PASS" : "FAIL";
        QColor statusColor = test.passed ? greenText : redText;

        drawTextBlock(QRect(innerX, innerY, 200, 20),
                      "Status: " + statusText,
                      bodyFont,
                      statusColor);

        drawTextBlock(QRect(innerX + 250, innerY, 300, 20),
                      "Duration: " + QString::number(test.durationMs, 'f', 2) + " ms",
                      bodyFont,
                      mutedText);

        y += boxHeight + 20;
    }

    // ============================================================
    // NOTES SECTION (ONLY SHOW IF REAL NOTES EXIST)
    // ============================================================
    if (!cleanedMessages.isEmpty()) {
        drawTextBlock(QRect(leftMargin, y, contentWidth, 28),
                      "Test Notes",
                      sectionTitleFont,
                      navyText);

        y += 40;

        int notesHeight = qMin(320, 70 + cleanedMessages.size() * 42);
        QRect notesRect(leftMargin, y, contentWidth, notesHeight);
        drawRoundedBox(notesRect, QColor("#ffffff"), lightBorder);

        int messageY = y + 18;
        for (int i = 0; i < cleanedMessages.size(); ++i) {
            if (messageY + 48 > y + notesHeight - 12)
                break;

            QRect bulletRect(leftMargin + 20, messageY + 6, 14, 14);
            painter.save();
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor("#2563eb"));
            painter.drawEllipse(bulletRect);
            painter.restore();

            drawTextBlock(QRect(leftMargin + 44, messageY, contentWidth - 64, 36),
                          cleanedMessages[i],
                          bodyFont,
                          navyText,
                          Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap);

            messageY += 42;
        }

        y += notesHeight + 30;
    }

    // ============================================================
    // FOOTER
    // ============================================================
    painter.save();
    painter.setPen(QPen(lightBorder, 1));
    painter.drawLine(leftMargin, pageHeight - 90, pageWidth - rightMargin, pageHeight - 90);
    painter.restore();

    drawTextBlock(QRect(leftMargin, pageHeight - 78, contentWidth, 20),
                  "Circuit GUI Prototype - PDF Report",
                  smallFont,
                  mutedText,
                  Qt::AlignLeft | Qt::AlignVCenter);

    drawTextBlock(QRect(leftMargin, pageHeight - 78, contentWidth, 20),
                  fileInfo.fileName(),
                  smallFont,
                  mutedText,
                  Qt::AlignRight | Qt::AlignVCenter);

    painter.end();

    QFileInfo pdfInfo(filePath);
    if (!pdfInfo.exists()) {
        emit reportFailed("PDF file was not created: " + filePath);
        return false;
    }

    if (pdfInfo.size() <= 0) {
        emit reportFailed("PDF file is empty: " + filePath);
        return false;
    }

    emit reportGenerated(filePath);
    return true;
}