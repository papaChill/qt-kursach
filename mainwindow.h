#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ht.h"
#include <QMainWindow>
#include "tree.h"
#include "coaches.h"
#include "workoututils.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_action_triggered();
    void addTrainer();
    QString capitalizeFirstt(const QString& str);
    void showTrainersInTable();
    void on_action_2_triggered();
    void removeRecord();
    void on_action_3_triggered();
    void findRecord();
    void on_action_4_triggered();
    void on_action_5_triggered();
    void showTrainersArrayInTable();
    void on_action_7_triggered();
    void loadFromFile();
    void on_action_6_triggered();
    void on_action_8_triggered();
    void addRecordToAVL();
    void showWorkoutsInTable();
    void on_action_9_triggered();
    void findWorkout();
    void on_action_10_triggered();
    void deleteWorkout();
    void on_action_11_triggered();
    void showWorkoutsFromArray();
    void on_action_12_triggered();
    void on_action_13_triggered();
    void saveWorkoutStorageToFile();
    void on_action_14_triggered();
    void loadWorkoutFromFile();

    void on_action_16_triggered();
    QString buildAvlTreeString(avl::node* nod, int level);
    void on_action_17_triggered();
    void on_action_15_triggered();
    void report();

    void on_action_18_triggered();
    QString avlToQString(avl::node* nod, const workoutStorage& storage, int level = 0);

    void on_action_19_triggered();
    void on_action_20_triggered();

private:
    Ui::MainWindow *ui;

    HashTable* table = nullptr;
    avl* avlTree = nullptr;
    coachesStorage CoachesStorage;
    workoutStorage WorkoutStorage;
    avl* reportTree=nullptr;
};
#endif // MAINWINDOW_H
