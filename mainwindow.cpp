#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ht.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <locale>
#include <QTextCodec>
#include <QTextStream>
#include <QThread>
#include "coachesutils.h"
#include "constants.h"
#include "workoututils.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    addTrainer();
}

void MainWindow::addTrainer()
{
    // 1. Проверка создания таблицы
    if (!table) {
        bool ok = false;
        int size = QInputDialog::getInt(this, "Создание хеш-таблицы", "Введите размер хеш-таблицы:", 25, 1, 10000, 1, &ok);
        if (!ok) return;
        table = new HashTable(size);
    }

    // Регулярное выражение: только буквы и дефис
    QRegularExpression nameRegex("^[A-Za-zА-Яа-яЁё\\-]+$");

    bool ok;

    // 2. Единый ввод ФИО
    QString fullFio = QInputDialog::getText(this, "Добавить тренера",
                                            "Введите ФИО (Фамилия Имя Отчество) через пробел:",
                                            QLineEdit::Normal, "", &ok);
    if (!ok || fullFio.isEmpty()) return;

    // Разбиваем строку по пробелам, игнорируя пустые (если случайно ввели два пробела подряд)
    QStringList parts = fullFio.split(' ', Qt::SkipEmptyParts);

    // ПРОВЕРКА КОЛИЧЕСТВА СЛОВ
    if (parts.size() != 3) {
        QMessageBox::warning(this, "Ошибка ввода",
                             "ФИО должно состоять ровно из трёх слов!\n"
                             "Формат: Фамилия Имя Отчество");
        return;
    }

    QString surname = parts[0];
    QString name = parts[1];
    QString patronymic = parts[2];

    // ПРОВЕРКА НА БУКВЫ ДЛЯ КАЖДОЙ ЧАСТИ
    if (!nameRegex.match(surname).hasMatch()) {
        QMessageBox::warning(this, "Ошибка ввода", "Фамилия должна содержать только буквы!");
        return;
    }
    if (!nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, "Ошибка ввода", "Имя должно содержать только буквы!");
        return;
    }
    if (!nameRegex.match(patronymic).hasMatch()) {
        QMessageBox::warning(this, "Ошибка ввода", "Отчество должно содержать только буквы!");
        return;
    }

    // ФОРМАТИРОВАНИЕ (Первая заглавная, остальные строчные)
    // Использую твою функцию capitalizeFirstt (с двумя t, как у тебя в коде)
    surname = capitalizeFirstt(surname);
    name = capitalizeFirstt(name);
    patronymic = capitalizeFirstt(patronymic);

    // 3. Ввод Специализации (ВЫБОР ИЗ СПИСКА)
    QStringList items;
    items << "Бокс" << "Борьба" << "Гимнастика" << "Дзюдо" << "Кикбоксинг" << "ЛегкаяАтлетика" << "ММА" << "Танцы";

    QString spec = QInputDialog::getItem(this, "Добавить тренера",
                                         "Выберите специализацию:",
                                         items, 0, false, &ok);
    if (!ok || spec.isEmpty()) return;

    // 4. Ввод Стажа
    int exp = QInputDialog::getInt(this, "Добавить тренера", "Введите стаж (в годах):", 1, 0, 40, 1, &ok);
    if (!ok) return;

    // 5. Создание записи и вставка
    Record r;
    r.surname = surname.toStdString();
    r.name = name.toStdString();
    r.patronymic = patronymic.toStdString();
    r.spec = spec.toStdString();
    r.exp = exp;

    QString er;
    bool da = CoachesUtils::insertCoaches(r, *table, CoachesStorage, &er);

    if (da) {
        QMessageBox::information(this, "Успех", "Тренер успешно добавлен!");
        showTrainersInTable();
    } else {
        QString errorMsg = "Не удалось добавить запись.";
        if (er == "duplicate") errorMsg += "\nПричина: Такой тренер уже существует.";
        else if (er == "overflow") errorMsg += "\nПричина: Таблица переполнена.";
        else if (!er.isEmpty()) errorMsg += "\nПричина: " + er;

        QMessageBox::warning(this, "Ошибка", errorMsg);
    }
}

QString MainWindow::capitalizeFirstt(const QString& str) {
    if (str.isEmpty()) return str;
    return str.left(1).toUpper() + str.mid(1).toLower();
}

void MainWindow::showTrainersInTable()
{
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Хеш-таблица не создана.");
        return;
    }

    int n = table->size;

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(n);
    ui->tableWidget->setColumnCount(4);

    QStringList headers;
    headers << "Индекс в ХТ" << "ФИО (из массива)" << "Специализация / Стаж" << "Статус";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < n; ++i) {
        const HashEntry& entry = table->table[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i)));

        if (entry.state == OCCUPIED) {
            int idx = entry.data;
            if (idx >= 0 && idx < CoachesStorage.coachesCount) {
                const Record& r = CoachesStorage.coaches[idx];
                QString fio = QString::fromStdString(r.surname + " " + r.name + " " + r.patronymic);
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(fio));
                QString info = QString::fromStdString(r.spec)+ " " + QString::number(r.exp);
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(info));
            } else {
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem("Некорректный индекс"));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
            }
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem("1"));
        }
        else if (entry.state == DELETED) {
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("---"));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem("2"));
        }
        else {
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("---"));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem("0"));
        }
    }
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_action_2_triggered()
{
    removeRecord();
}
void MainWindow::removeRecord()
{
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Сначала создайте или загрузите справочник тренеров (хеш-таблицу)!");
        return;
    }
    if (table->isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренеров пуст!");
        return;
    }
    bool ok;
    Record r;
    r.surname = QInputDialog::getText(this, "Удалить тренера", "Введите фамилию:", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok || r.surname.empty()) return;
    r.name = QInputDialog::getText(this, "Удалить тренера", "Введите имя:", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok || r.name.empty()) return;
    r.patronymic = QInputDialog::getText(this, "Удалить тренера", "Введите отчество:", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok || r.patronymic.empty()) return;
    r.spec = QInputDialog::getText(this, "Удалить тренера", "Введите специальность:", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok || r.spec.empty()) return;
    r.exp = QInputDialog::getInt(this, "Удалить тренера", "Введите стаж (лет):", 0, 0, 100, 1, &ok);
    if (!ok) return;
    if(avlTree){
        bool da=avlTree->findNode(avlTree->root, r.get_key());
        if(da){
            QDate date = QDate(0, 0, 0);
            string key = r.get_key();
            QString errorReason;
            WorkoutUtils::removeWorkout(key, date, *avlTree, WorkoutStorage, &errorReason);
            MainWindow::showWorkoutsInTable();
        }
    }
    bool deleted = CoachesUtils::removeCoaches(r, *table, CoachesStorage);
    if (deleted) {
        QMessageBox::information(this, "Удаление", "Тренер успешно удалён из справочника!");
        showTrainersInTable();
    } else {
        QMessageBox::warning(this, "Ошибка", "Такой тренер не найден в справочнике!");
    }
}

void MainWindow::on_action_3_triggered()
{
    findRecord();
}

void MainWindow::findRecord()
{
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите справочник тренеров (хеш-таблицу)!");
        return;
    }
    if (table->isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренеров пуст!");
        return;
    }
    bool ok;
    QString input = QInputDialog::getText(this, "Найти тренера","Введите ФИО (Фамилия Имя Отчество) через пробел:",QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) return;
    QStringList parts = input.split(' ', Qt::SkipEmptyParts);

    if (parts.size() != 3) {
        QMessageBox::warning(this, "Ошибка", "Введите ровно три слова: Фамилия Имя Отчество!");
        return;
    }
    QString surname = capitalizeFirstt(parts[0]);
    QString name    = capitalizeFirstt(parts[1]);
    QString patronymic = capitalizeFirstt(parts[2]);

    string fio = (surname + " " + name + " " + patronymic).toStdString();

    Record* r = CoachesUtils::find(fio, *table, CoachesStorage);

    if (r != nullptr && r->get_key() == fio) {
        QMessageBox::information(this, "Найдено", QString("ФИО: %1 %2 %3\nСпециализация: %4\nСтаж: %5")
                                     .arg(QString::fromStdString(r->surname))
                                     .arg(QString::fromStdString(r->name))
                                     .arg(QString::fromStdString(r->patronymic))
                                     .arg(QString::fromStdString(r->spec))
                                     .arg(r->exp));
    } else {
        QMessageBox::warning(this, "Не найдено", "Такой тренер не найден!");
    }
}

void MainWindow::showTrainersArrayInTable() {
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    QStringList headers;
    headers << "ФИО" << "Специальность" << "Стаж";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    int row = 0;
    for (int i = 0; i < table->getSize(); ++i) {
        HashEntry& entry = table->table[i];
        if (entry.state==1){
            int idx = entry.data;

            if (idx >= 0 && idx < CoachesStorage.coachesCount) {
                const Record& r = CoachesStorage.coaches[idx];
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(r.name +" " + r.surname +" "+r.patronymic)));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(r.spec)));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(r.exp)));
                row++;
            }
        }
    }
}


void MainWindow::on_action_4_triggered() {
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите справочник номеров (хеш-таблицу)!");
        return;
    }
    showTrainersArrayInTable();
}

void MainWindow::on_action_5_triggered() {
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите справочник номеров (хеш-таблицу)!");
        return;
    }
    showTrainersInTable();
}

void MainWindow::on_action_7_triggered()
{
    loadFromFile();
}

void MainWindow::loadFromFile()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QCoreApplication::processEvents();
    QThread::msleep(150);
    if (table) {
        table->clear();
        delete table;
        table = nullptr;
    }
    if (avlTree) {
        // рекурсивно обходим дерево и удаляем индексы из статического массива
        WorkoutUtils::clearAvlTree(avlTree->root, WorkoutStorage);
        delete avlTree;
        avlTree = nullptr;
    }

    for (int i = 0; i < WorkoutStorage.workoutcount; ++i) {
        WorkoutStorage.workout[i] = WorkOut();
    }
    WorkoutStorage.workoutcount = 0;
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    bool ok = false;
    int size = QInputDialog::getInt(this, "Размер хеш-таблицы", "Введите размер хеш-таблицы:", 25, 1, 10000, 1, &ok);
    if (!ok) return;

    table = new HashTable(size);
    QString filename = QFileDialog::getOpenFileName(this, "Выберите файл со списком тренеров", "", "Текстовые файлы (*.txt);;Все файлы (*.*)");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка файла", QString("Не удалось открыть файл %1").arg(filename));
        return;
    }

    QTextStream in(&file);
    QStringList errorLines;
    int validCount = 0, invalidCount = 0, duplicates = 0, overflow = 0;
    int validTotal = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        if (parts.size() < 5) {
            errorLines << QString("%1 — Ошибка: недостаточно полей (ожидалось 5: фамилия имя отчество спец стаж)").arg(line);
            invalidCount++;
            continue;
        }

        QString surname = parts[0];
        QString name = parts[1];
        QString patronymic = parts[2];
        QString spec = parts[3];
        bool okExp = false;
        int exp = parts[4].toInt(&okExp);
        if (!okExp || exp < 0 || exp > 40) {
            errorLines << QString("%1 — Ошибка: некорректный стаж (0–100)").arg(line);
            invalidCount++;
            continue;
        }

        Record r;
        r.surname = surname.toStdString();
        r.name = name.toStdString();
        r.patronymic = patronymic.toStdString();
        r.spec = spec.toStdString();
        r.exp = exp;

        validTotal++;
        QString err;
        bool added = CoachesUtils::insertCoaches(r, *table, CoachesStorage, &err);
        if (added) {
            validCount++;
        } else {
            if (err.contains("duplicate", Qt::CaseInsensitive)) {
                duplicates++;
                errorLines << QString("%1 — Ошибка: дубликат записи").arg(line);
            } else if (err.contains("overflow", Qt::CaseInsensitive)) {
                overflow++;
                errorLines << QString("%1 — Ошибка: таблица переполнена").arg(line);
            } else {
                invalidCount++;
                errorLines << QString("%1 — Ошибка: %2").arg(line, err);
            }
        }
    }
    file.close();
    QString message = QString("Всего строк: %1\n" "Корректных записей: %2\n" "Добавлено успешно: %3\n" "Пропущено (дубликаты): %4\n"
                          "Пропущено (переполнение): %5\n" "Некорректных строк: %6").arg(validTotal + invalidCount)
                          .arg(validTotal).arg(validCount).arg(duplicates).arg(overflow).arg(invalidCount);
    QMessageBox::information(this, "Результаты загрузки", message);

    showTrainersInTable();
}


void MainWindow::on_action_6_triggered()
{
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренеров не создан.");
        return;
    }
    if (table->isEmpty()) {
        QMessageBox::information(this, "Сохранение", "Нет данных для сохранения.");
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить справочник тренеров", "", "Текстовые файлы (*.txt)");
    if (filename.isEmpty()) return;
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка файла", QString("Не удалось открыть файл %1").arg(filename));
        return;
    }
    QTextStream out(&file);
    int savedCount = 0;
    for (int i = 0; i < CoachesStorage.coachesCount; ++i) {
        const Record& r = CoachesStorage.coaches[i];
        out << QString::fromStdString(r.surname) << " "
            << QString::fromStdString(r.name) << " "
            << QString::fromStdString(r.patronymic) << " "
            << QString::fromStdString(r.spec) << " "
            << r.exp << "\n";
        savedCount++;
    }
    file.close();
    if (savedCount == 0) {
        QMessageBox::information(this, "Сохранение", "Нет данных для сохранения.");
    } else {
        QMessageBox::information(this, "Сохранение", QString("Справочник успешно сохранен.\nСохранено %1 записей.") .arg(savedCount));
    }
}

void MainWindow::on_action_8_triggered()
{
    addRecordToAVL();
}
void MainWindow::addRecordToAVL()
{
    // 1. Проверка наличия справочника тренеров
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренеров не создан.");
        return;
    }
    bool ok;
    QString errorReason;

    // ===========================================================================
    // ШАГ 1: ВВОД И ВАЛИДАЦИЯ ФИО
    // ===========================================================================
    QString fioInput = QInputDialog::getText(this, "Добавить запись",
                                             "Введите ФИО тренера (Фамилия Имя Отчество):",
                                             QLineEdit::Normal, "", &ok);
    if (!ok || fioInput.isEmpty()) return;

    QStringList parts = fioInput.split(" ", Qt::SkipEmptyParts);

    if (parts.size() != 3) {
        QMessageBox::warning(this, "Ошибка", "ФИО должно состоять ровно из трёх слов!\nФормат: Фамилия Имя Отчество");
        return;
    }

    QString surname = parts[0];
    QString name = parts[1];
    QString patronymic = parts[2];

    QRegularExpression nameRegex("^[A-Za-zА-Яа-яЁё\\-]+$");
    if (!nameRegex.match(surname).hasMatch() ||
        !nameRegex.match(name).hasMatch() ||
        !nameRegex.match(patronymic).hasMatch())
    {
        QMessageBox::warning(this, "Ошибка ввода", "ФИО должно содержать только буквы!");
        return;
    }

    surname = capitalizeFirstt(surname);
    name = capitalizeFirstt(name);
    patronymic = capitalizeFirstt(patronymic);

    string fioStr = surname.toStdString() + " " + name.toStdString() + " " + patronymic.toStdString();

    Record* found = CoachesUtils::find(fioStr, *table, CoachesStorage);
    if (found->get_key() != fioStr) {
        QMessageBox::warning(this, "Ошибка", "Такого тренера нет в справочнике. Добавление тренировки запрещено!");
        return;
    }

    // ===========================================================================
    // ШАГ 2: ВЫБОР ГРУППЫ (ИЗ СПИСКА)
    // ===========================================================================
    QStringList groupList;
    groupList << "A-01" << "B-02" << "C-03" << "D-04" << "E-05";

    QString group = QInputDialog::getItem(this, "Добавить запись",
                                          "Выберите группу:",
                                          groupList, 0, false, &ok);
    if (!ok || group.isEmpty()) return;


    // ===========================================================================
    // ШАГ 3: ВВОД И ВАЛИДАЦИЯ ВРЕМЕНИ (10:00 - 19:59)
    // ===========================================================================
    QString timeStr = QInputDialog::getText(this, "Добавить запись",
                                            "Введите время (HH:mm):",
                                            QLineEdit::Normal, "", &ok);
    if (!ok || timeStr.isEmpty()) return;

    // Пробуем распарсить строку как время
    QTime t = QTime::fromString(timeStr, "HH:mm"); // или "H:m"

    // 1. Проверка формата и валидности минут (QTime сам не даст сделать 65 минут)
    if (!t.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат времени! Используйте HH:mm (например, 14:30).\nМинуты должны быть от 00 до 59. Часы от 10 до 21");
        return;
    }

    // 2. Проверка диапазона часов (с 10 до 19 включительно)
    if (t.hour() < 10 || t.hour() > 20) {
        QMessageBox::warning(this, "Ошибка", "Время тренировки должно быть в диапазоне от 10:00 до 20:59!");
        return;
    }

    // ===========================================================================
    // ШАГ 4: ВВОД ДАТЫ
    // ===========================================================================
    QString date = QInputDialog::getText(this, "Добавить запись", "Введите дату (dd.MM.yyyy):", QLineEdit::Normal, "", &ok);
    if (!ok || date.isEmpty()) return;

    QDate d = QDate::fromString(date, "dd.MM.yyyy");
    if (!d.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат даты. Ожидается dd.MM.yyyy");
        return;
    }

    // ===========================================================================
    // ШАГ 5: СОЗДАНИЕ ЗАПИСИ
    // ===========================================================================
    WorkOut note;
    note.surname    = found->surname;
    note.name       = found->name;
    note.patronymic = found->patronymic;
    note.group      = group.toStdString();
    note.time       = t.toString("HH:mm").toStdString(); // Сохраняем отформатированное валидное время
    note.date       = d;

    if (!avlTree) {
        avlTree = new avl();
    }

    WorkoutUtils::insertWorkout(note, *avlTree, WorkoutStorage, &errorReason);

    if (!errorReason.isEmpty()) {
        QString msg;
        if (errorReason == "storage_full") msg = "Массив тренировок переполнен!";
        else if (errorReason == "date_in_past") msg = "Нельзя добавлять тренировки на прошедшую дату!";
        else if (errorReason == "date_too_far") msg = "Дата тренировки не может быть больше чем через год!";
        else if (errorReason == "coach_busy") msg = "Этот тренер уже занят в указанное время!";
        else if (errorReason == "group_busy") msg = "Эта группа уже занимается в указанное время!";
        else msg = QString("Неизвестная ошибка: %1").arg(errorReason);

        QMessageBox::warning(this, "Ошибка добавления", msg);
        return;
    }

    showWorkoutsInTable();
}

void MainWindow::showWorkoutsInTable()
{
    List arr;
    avl::returnArray(avlTree->root, arr);
    List::Node* current=arr.head;
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setColumnCount(5);
    int totalRecords = arr.SizeL(/*&arr*/);
    ui->tableWidget_2->setRowCount(totalRecords);
    QStringList headers;
    headers << "ФИО (ключ)" << "Группа" << "Время" << "Дата" << "Индекс";
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);
    int row = 0;
    while(current!=nullptr){
        int idx=current->data;
        if (idx >= 0 && idx < WorkoutStorage.workoutcount) {
            const WorkOut& w = WorkoutStorage.workout[idx];
            QString fio   = QString::fromStdString(w.get_key());
            QString group = QString::fromStdString(w.group);
            QString time  = QString::fromStdString(w.time);
            QString date  = w.date.toString("dd.MM.yyyy");

            ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(fio));
            ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(group));
            ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(time));
            ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(date));
            ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(QString::number(idx)));
            row++;
        }
        current=current->pNext;
    }
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QMessageBox::information(this, "Отображение", QString("Показано %1 тренировок.").arg(totalRecords));
}

void MainWindow::on_action_9_triggered()
{
    findWorkout();
}

void MainWindow::findWorkout()
{
    if (!avlTree || !avlTree->root) {
        QMessageBox::warning(this, "Ошибка", "AVL дерево пустое!");
        return;
    }
    bool ok;
    QString fio = QInputDialog::getText(this, "Поиск тренировки", "Введите ФИО (Фамилия Имя Отчество):", QLineEdit::Normal, "", &ok);
    if (!ok || fio.isEmpty()) return;
    QString dateStr = QInputDialog::getText(this, "Поиск тренировки", "Введите дату (в формате dd.MM.yyyy):", QLineEdit::Normal, "", &ok);
    if (!ok || dateStr.isEmpty()) return;
    QDate date = QDate::fromString(dateStr, "dd.MM.yyyy");
    if (!date.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат даты! Ожидается dd.MM.yyyy");
        return;
    }
    string key = fio.toStdString();
    QString errorReason;
    WorkOut found = WorkoutUtils::findByDate(key, date, *avlTree, WorkoutStorage, &errorReason);
    if (errorReason == "empty_tree") {
        QMessageBox::warning(this, "Ошибка", "AVL дерево пустое!");
    } else if (errorReason == "not_found") {
        QMessageBox::information(this, "Результат", "Тренер с таким ФИО не найден.");
    } else if (errorReason == "no_match_date") {
        QMessageBox::information(this, "Результат", "Для этого тренера нет тренировки с указанной датой.");
    } else {
        QString info = QString("ФИО: %1\n""Группа: %2\n""Время: %3\n""Дата: %4").arg(QString::fromStdString(found.get_key()))
        .arg(QString::fromStdString(found.group)).arg(QString::fromStdString(found.time)).arg(found.date.toString("dd.MM.yyyy"));
        QMessageBox::information(this, "Найденная тренировка", info);
    }
}

void MainWindow::on_action_10_triggered()
{
    deleteWorkout();
}

void MainWindow::deleteWorkout()
{
    if (!avlTree || !avlTree->root) {
        QMessageBox::warning(this, "Ошибка", "AVL дерево пустое!");
        return;
    }
    bool ok;
    QString fio = QInputDialog::getText(this, "Удаление тренировки", "Введите ФИО (Фамилия Имя Отчество):", QLineEdit::Normal, "", &ok);
    if (!ok || fio.isEmpty()) return;
    QString dateStr = QInputDialog::getText(this, "Удаление тренировки", "Введите дату (в формате dd.MM.yyyy):", QLineEdit::Normal, "", &ok);
    if (!ok || dateStr.isEmpty()) return;
    QDate date = QDate::fromString(dateStr, "dd.MM.yyyy");
    if (!date.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Неверный формат даты! Ожидается dd.MM.yyyy");
        return;
    }
    string key = fio.toStdString();
    QString errorReason;
    WorkoutUtils::removeWorkout(key, date, *avlTree, WorkoutStorage, &errorReason);
    if (errorReason == "empty_tree") {
        QMessageBox::warning(this, "Ошибка", "AVL дерево пустое!");
    } else if (errorReason == "not_found") {
        QMessageBox::information(this, "Результат", "Тренер с таким ФИО не найден!");
    } else if (errorReason == "no_match_date") {
        QMessageBox::information(this, "Результат", "У этого тренера нет тренировки на указанную дату!");
    } else {
        QMessageBox::information(this, "Успешно", "Тренировка успешно удалена!");
        showWorkoutsInTable();
    }
}

void MainWindow::on_action_11_triggered()
{
    showWorkoutsFromArray();
}

void MainWindow::showWorkoutsFromArray()
{
    if (WorkoutStorage.workoutcount == 0) {
        QMessageBox::information(this, "Информация", "В хранилище тренировок нет данных!");
        return;
    }
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setColumnCount(5);
    ui->tableWidget_2->setHorizontalHeaderLabels({"ФИО", "Группа", "Время", "Дата", "Индекс"});
    ui->tableWidget_2->setRowCount(WorkoutStorage.workoutcount);
    for (int i = 0; i < WorkoutStorage.workoutcount; ++i) {
        const WorkOut& w = WorkoutStorage.workout[i];
        QString fio   = QString::fromStdString(w.get_key());
        QString group = QString::fromStdString(w.group);
        QString time  = QString::fromStdString(w.time);
        QString date  = w.date.toString("dd.MM.yyyy");
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(fio));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(group));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(time));
        ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(date));
        ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(QString::number(i)));
    }
    ui->tableWidget_2->resizeColumnsToContents();
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::on_action_12_triggered()
{
    if (WorkoutStorage.workoutcount == 0) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренировок пуст или не загружен!");
        return;
    }
    if (!avlTree || !avlTree->root) {
        QMessageBox::warning(this, "Ошибка", "Авл-дерево не создано!");
        return;
    }
    showWorkoutsInTable();
}

void MainWindow::on_action_13_triggered()
{
    saveWorkoutStorageToFile();
}

void MainWindow::saveWorkoutStorageToFile()
{
    if (WorkoutStorage.workoutcount == 0) {
        QMessageBox::information(this, "Информация", "Хранилище тренировок пусто, сохранять нечего.");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить справочник тренировок", "", "Текстовые файлы (*.txt);;Все файлы (*.*)" );
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи.");
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < WorkoutStorage.workoutcount; ++i) {
        const WorkOut& w = WorkoutStorage.workout[i];
        out << QString::fromStdString(w.get_key()) << ";"
            << QString::fromStdString(w.group) << ";"
            << QString::fromStdString(w.time)  << ";"
            << w.date.toString("dd.MM.yyyy")   << "\n";
    }
    file.close();
    QMessageBox::information(this, "Сохранено", "Справочник тренировок успешно сохранён!");
}

void MainWindow::on_action_14_triggered()
{
    loadWorkoutFromFile();
}

void MainWindow::loadWorkoutFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Загрузить справочник тренировок", "", "Текстовые файлы (*.txt);;Все файлы (*.*)");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для чтения.");
        return;
    }
    QTextStream in(&file);
    WorkoutStorage.workoutcount = 0;
    if (avlTree) { delete avlTree; avlTree = nullptr; }
    avlTree = new avl();

    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Хеш-таблица тренеров не инициализирована!");
        file.close();
        return;
    }

    int lineNumber = 0;
    QString errorReason;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        ++lineNumber;
        if (line.isEmpty()) continue;

        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() < 6) {
            continue;
        }

        WorkOut note;
        note.surname    = parts[0].toStdString();
        note.name       = parts[1].toStdString();
        note.patronymic = parts[2].toStdString();
        note.group      = parts[3].toStdString();
        note.time       = parts[4].toStdString();
        note.date       = QDate::fromString(parts[5], "dd.MM.yyyy");
        if (!note.date.isValid()) {
            continue;
        }

        // --- СТРОГО ПРОВЕРЯЕМ НАЛИЧИЕ ТРЕНЕРА ПО get_key() ---
        std::string fioKey = note.surname + " " + note.name + " " + note.patronymic;
        int htIdx = table->search(fioKey); // возвращает индекс в CoachStorage (или 0/ - ? — в твоей реализации)
        if (htIdx < 0 || htIdx >= CoachesStorage.coachesCount) {
            continue; // пропускаем запись
        }
        // Дополнительная защита: убеждаемся, что в CoachStorage по этому индексу именно тот ключ
        Record& r = CoachesStorage.coaches[htIdx];
        if (r.get_key() != fioKey) {
            continue;
        }

        // Всё ок — вставляем
        WorkoutUtils::insertWorkout(note, *avlTree, WorkoutStorage, &errorReason);
        if (!errorReason.isEmpty()) {
            if (errorReason == "storage_full") {
                QMessageBox::warning(this, "Ошибка", "Хранилище тренировок переполнено!");
                break;
            } else {
                QMessageBox::warning(this, "Ошибка", QString("Ошибка при добавлении (строка %1): %2").arg(lineNumber).arg(errorReason));
            }
            errorReason.clear();
        }
    }
    file.close();
    QMessageBox::information(this, "Загрузка завершена", QString("Загружено %1 записей.").arg(WorkoutStorage.workoutcount));
    showWorkoutsInTable();
}

QString MainWindow::buildAvlTreeString(avl::node* nod, int level)
{
    if (!nod) return "";
    QString result;

    // 1. Рекурсия вправо
    result += buildAvlTreeString(nod->right, level + 1);

    // 2. Отступы
    for (int i = 0; i < level; i++)
        result += "    ";

    // 3. Вывод ключа (ФИО)
    result += QString::fromStdString(nod->key);

    // 4. Вывод списка тренировок в одну строку
    if (nod->ids) {
        List::Node* cur = nod->ids->head;
        bool isFirst = true;

        while (cur) {
            int idx = cur->data;

            // Проверка индекса
            if (idx >= 0 && idx < WorkoutStorage.workoutcount) {
                const WorkOut& w = WorkoutStorage.workout[idx];

                // Формируем строку: "Группа Время Дата"
                QString info = QString("%1  %2  %3")
                                   .arg(QString::fromStdString(w.group))
                                   .arg(QString::fromStdString(w.time))
                                   .arg(w.date.toString("dd.MM.yyyy"));

                // Если это первая запись — ставим стрелочку "->", если последующие — просто пробел
                if (isFirst) {
                    result += " -> " + info + " {" + QString::number(idx) + "}";
                } else {
                    result += "  " + info + " {" + QString::number(idx) + "}";
                }
            }
            cur = cur->pNext;
            isFirst = false;
        }
    }

    result += "\n";

    // 5. Рекурсия влево
    result += buildAvlTreeString(nod->left, level + 1);

    return result;
}

void MainWindow::on_action_16_triggered()
{
    if (!avlTree) {
        QMessageBox::warning(this, "Ошибка", "AVL-дерево не создано!");
        return;
    }

    if (!avlTree->root) {
        QMessageBox::information(this, "Инфо", "Дерево пустое.");
        ui->textEdit->clear();
        return;
    }

    QString treeText = buildAvlTreeString(avlTree->root, 0);
    ui->textEdit->setPlainText(treeText);
}

void MainWindow::on_action_17_triggered()
{
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Хеш-таблица не создана!");
        return;
    }

    QString result;
    result += "=== Отладка хеширования ===\n\n";

    for (int i = 0; i < CoachesStorage.coachesCount; ++i) {
        const Record& rec = CoachesStorage.coaches[i];
        string key = rec.get_key();
        int h0 = table->primaryHash(key);

        result += QString("запись %1\n").arg(QString::fromStdString(key));
        result += QString("первичный хеш: h0 = %1\n").arg(h0);

        // Проверяем, были ли коллизии
        int idx = h0;
        int j = 0;
        while (j < table->size && table->table[idx].state != EMPTY) {
            if (table->table[idx].key == key) break;

            int h1 = table->secondaryHash(h0, j + 1);
            if (j == 0)
                result += QString("вторичный хеш: h%1 = (h0 + %1) %% %2 = %3\n")
                              .arg(j + 1)
                              .arg(table->size)
                              .arg(h1);
            else
                result += QString("вторичный хеш (попытка %1): h%2 = (h0 + %2) %% %3 = %4\n")
                              .arg(j + 1)
                              .arg(j + 1)
                              .arg(table->size)
                              .arg(h1);
            idx = h1;
            j++;
        }
        result += "\n";
    }

    result += "======================================\n";

    ui->textEdit->setPlainText(result);
}

void MainWindow::on_action_15_triggered()
{
    report();
}

void collectByDateRange(avl::node* nod, const QDate& from, const QDate& to, List& result) {
    if (!nod) return;

    QDate nodeDate = QDate::fromString(QString::fromStdString(nod->key), "dd.MM.yyyy");
    if (!nodeDate.isValid()) return;

    // если возможны элементы слева — обходим
    if (nodeDate > from) collectByDateRange(nod->left, from, to, result);

    // если попадает в диапазон — добавляем все индексы узла в result
    if (nodeDate >= from && nodeDate <= to) {
        List::Node* cur = nod->ids ? nod->ids->head : nullptr;
        while (cur) {
            result.AddToEnd(cur->data);
            cur = cur->pNext;
        }
    }

    // если возможны элементы справа — обходим
    if (nodeDate < to) collectByDateRange(nod->right, from, to, result);
}

void MainWindow::report()
{
    if (WorkoutStorage.workoutcount == 0) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренировок пуст!");
        return;
    }
    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Хеш-таблица тренеров не инициализирована!");
        return;
    }

    // --- Очистка старого дерева отчёта ---
    if (reportTree) { delete reportTree; reportTree = nullptr; }
    reportTree = new avl();

    // --- Фильтры ---
    bool ok;
    QString group = QInputDialog::getText(this, "Фильтр", "Введите группу (или оставьте пустым):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString fromStr = QInputDialog::getText(this, "Фильтр", "Введите дату начала (дд.MM.гггг):", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    QString toStr = QInputDialog::getText(this, "Фильтр", "Введите дату окончания (дд.MM.гггг):", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QDate fromDate = QDate::fromString(fromStr, "dd.MM.yyyy");
    QDate toDate   = QDate::fromString(toStr, "dd.MM.yyyy");
    if (!fromDate.isValid() || !toDate.isValid() || fromDate > toDate) {
        QMessageBox::warning(this, "Ошибка", "Неверный диапазон дат.");
        return;
    }

    int minExp = QInputDialog::getInt(this, "Фильтр", "Минимальный стаж (лет, 0 = любой):", 0, 0, 100, 1, &ok);
    if (!ok) return;

    // --- Сбор индексов тренировок, подходящих под фильтр ---
    QString reportText;
    reportText += "ФИО | Группа | Время | Дата | Специализация | Стаж\n";
    reportText += "---------------------------------------------------------------\n";

    int foundCount = 0;

    for (int i = 0; i < WorkoutStorage.workoutcount; ++i) {
        const WorkOut& w = WorkoutStorage.workout[i];
        if (!w.date.isValid()) continue;

        // проверяем, входит ли дата в диапазон
        if (w.date < fromDate || w.date > toDate) continue;

        // находим тренера
        std::string fioKey = w.surname + " " + w.name + " " + w.patronymic;
        Record* coach = CoachesUtils::find(fioKey, *table, CoachesStorage);
        if (!coach) continue;

        // применяем фильтры
        bool matchGroup = group.isEmpty() || QString::fromStdString(w.group) == group;
        bool matchExp   = (minExp == 0) || (coach->exp >= minExp);

        if (matchGroup && matchExp) {
            // добавляем в текст отчёта
            reportText += QString("%1 %2 %3 | %4 | %5 | %6 | %7 | %8 лет\n")
                              .arg(QString::fromStdString(w.surname))
                              .arg(QString::fromStdString(w.name))
                              .arg(QString::fromStdString(w.patronymic))
                              .arg(QString::fromStdString(w.group))
                              .arg(QString::fromStdString(w.time))
                              .arg(w.date.toString("dd.MM.yyyy"))
                              .arg(QString::fromStdString(coach->spec))
                              .arg(coach->exp);

            // добавляем в дерево отчёта
            std::string dateKey = w.date.toString("dd.MM.yyyy").toStdString();
            reportTree->insertt(dateKey, i);

            ++foundCount;
        }
    }

    // --- Вывод результата ---
    if (foundCount == 0) {
        QMessageBox::information(this, "Результат", "Не найдено подходящих тренировок по фильтрам.");
        ui->textEdit->clear();
    } else {
        ui->textEdit->setPlainText(reportText);
        QMessageBox::information(this, "Отчёт", QString("Отчёт успешно сформирован!\nНайдено записей: %1").arg(foundCount));
    }
}


QString MainWindow::avlToQString(avl::node* nod, const workoutStorage& storage, int level)
{
    if (!nod) return QString();

    QString result;
    result += avlToQString(nod->right, storage, level + 1);

    result += QString("    ").repeated(level);
    result += QString::fromStdString(nod->key) + " -> ";

    List::Node* cur = nod->ids ? nod->ids->head : nullptr;
    bool first = true;

    while (cur) {
        if (!first) result += ", ";
        first = false;

        int idx = cur->data;
        if (idx >= 0 && idx < storage.workoutcount) {
            const WorkOut& w = storage.workout[idx];
            result += QString("%1 %2 %3 %4 %5 {%6}")
                          .arg(QString::fromStdString(w.surname))
                          .arg(QString::fromStdString(w.name))
                          .arg(QString::fromStdString(w.patronymic))
                          .arg(QString::fromStdString(w.group))
                          .arg(QString::fromStdString(w.time))
                          .arg(idx);
        } else {
            result += QString("{%1}").arg(idx);
        }

        cur = cur->pNext;
    }

    result += "\n";
    result += avlToQString(nod->left, storage, level + 1);
    return result;
}

void MainWindow::on_action_18_triggered()
{
    if (WorkoutStorage.workoutcount == 0) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренировок пуст!");
        return;
    }

    // перестроим reportTree (удалим старое, если было)
    if (reportTree) {
        delete reportTree;
        reportTree = nullptr;
    }
    reportTree = new avl();
    for (int i = 0; i < WorkoutStorage.workoutcount; ++i) {
        const WorkOut& w = WorkoutStorage.workout[i];
        if (!w.date.isValid()) continue;
        string key = w.date.toString("dd.MM.yyyy").toStdString();
        reportTree->insertt(key, i);
    }
    QString treeText = avlToQString(reportTree->root, WorkoutStorage);
    ui->textEdit->setPlainText(treeText);
}


void MainWindow::on_action_19_triggered()
{
    if (!reportTree || WorkoutStorage.workoutcount == 0) {
        QMessageBox::warning(this, "Ошибка", "Сначала сформируйте отчёт по тренировкам!");
        return;
    }

    QString report;
    report += "ФИО | Группа | Время | Дата | Специализация | Стаж\n";
    report += "-------------------------------------------------------------\n";

    // Проходим все узлы AVL дерева (обход по возрастанию даты)
    std::function<void(avl::node*)> traverse = [&](avl::node* nod) {
        if (!nod) return;
        traverse(nod->left);

        List::Node* cur = nod->ids ? nod->ids->head : nullptr;
        while (cur) {
            int idx = cur->data;
            if (idx >= 0 && idx < WorkoutStorage.workoutcount) {
                const WorkOut& w = WorkoutStorage.workout[idx];
                std::string fio = w.surname + " " + w.name + " " + w.patronymic;

                // Ищем тренера по ФИО в хеш-таблице
                Record* coach = CoachesUtils::find(fio, *table, CoachesStorage);

                QString spec = coach ? QString::fromStdString(coach->spec) : "—";
                QString exp  = coach ? QString::number(coach->exp) : "—";

                report += QString("%1 %2 %3 | %4 | %5 | %6 | %7 | %8\n")
                              .arg(QString::fromStdString(w.surname))
                              .arg(QString::fromStdString(w.name))
                              .arg(QString::fromStdString(w.patronymic))
                              .arg(QString::fromStdString(w.group))
                              .arg(QString::fromStdString(w.time))
                              .arg(w.date.toString("dd.MM.yyyy"))
                              .arg(spec)
                              .arg(exp);
            }
            cur = cur->pNext;
        }

        traverse(nod->right);
    };

    traverse(reportTree->root);

    if (report.trimmed().isEmpty()) {
        QMessageBox::information(this, "Результат", "Нет данных для отображения.");
        return;
    }

    // Окно отчёта
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Отчёт по тренировкам и тренерам");
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(report);
    layout->addWidget(textEdit);
    dialog->setLayout(layout);
    dialog->resize(800, 600);
    dialog->exec();
}
void MainWindow::on_action_20_triggered()
{
    // 1. Проверки наличия данных
    if (!reportTree) {
        QMessageBox::warning(this, "Ошибка", "Отчётное дерево отсутствует!");
        return;
    }

    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Справочник тренеров (хеш-таблица) не загружен!\nНевозможно получить данные о специализации и стаже.");
        return;
    }

    if (WorkoutStorage.workoutcount == 0) {
        QMessageBox::warning(this, "Ошибка", "Нет тренировок для сохранения!");
        return;
    }

    // 2. Выбор файла
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить отчёт",
        "report.txt",
        "Текстовые файлы (*.txt);;Все файлы (*)"
        );

    if (filePath.isEmpty()) {
        return; // Отмена
    }

    // 3. Получаем все индексы из дерева (чтобы сохранить сортировку по алфавиту/дереву)
    List allIndexes;
    reportTree->returnArray(reportTree->root, allIndexes);

    if (!allIndexes.head) {
        QMessageBox::warning(this, "Ошибка", "В отчётном дереве нет записей!");
        return;
    }

    // 4. Открываем файл
    std::ofstream fout(filePath.toStdString().c_str(), std::ios::out | std::ios::trunc);
    if (!fout.is_open()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи!");
        return;
    }

    fout << "Формат: №) ФИО | Группа | Время | Дата | Специализация | Стаж\n\n";

    List::Node* cur = allIndexes.head;
    int num = 1;

    while (cur) {
        int idx = cur->data;
        if (idx >= 0 && idx < WorkoutStorage.workoutcount) {
            WorkOut& w = WorkoutStorage.workout[idx];

            if (!w.surname.empty()) {
                // Собираем ФИО для поиска в хеш-таблице
                string fioKey = w.surname + " " + w.name + " " + w.patronymic;

                // Ищем тренера в хеш-таблице, чтобы узнать Спец. и Стаж
                Record* coachRec = CoachesUtils::find(fioKey, *table, CoachesStorage);

                string specStr = "Нет данных";
                string expStr = "-";

                // Если нашли и ключ совпадает (исключаем коллизии/ошибки)
                if (coachRec && coachRec->get_key() == fioKey) {
                    specStr = coachRec->spec;
                    expStr = to_string(coachRec->exp);
                }

                // ВЫВОД В ОДНУ СТРОКУ
                fout << num++ << ") "
                     << w.surname << " " << w.name << " " << w.patronymic << " | "
                     << w.group << " | "
                     << w.time << " | "
                     << w.date.toString("dd.MM.yyyy").toStdString() << " | "
                     << specStr << " | "
                     << expStr << "\n";
            }
        }
        cur = cur->pNext;
    }

    fout.close();
    QMessageBox::information(this, "Успех", QString("Отчёт успешно сохранён в файл:\n%1").arg(filePath));
}
