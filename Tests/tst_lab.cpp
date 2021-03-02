#include <QtTest>

// add necessary includes here
#include <filesystem>

#include "../src/FileInfo/fileinfo.hpp"

class lab : public QObject
{
    Q_OBJECT

public:
    lab();
    ~lab();

private slots:
    void test_case1();
    void test_filesystem();
};

lab::lab()
{

}

lab::~lab()
{

}

void lab::test_case1()
{
    QVERIFY(true); // check that a condition is satisfied
    QCOMPARE(1, 1); // compare two values
}

void lab::test_filesystem()
{
    auto path = QString::fromStdString(std::filesystem::current_path().string()) + QDir::separator() + "file.check";
    QFile file(path);
    FileInfo fileInfo("E:\\Downloads\\Лабороторна_2_Довбуш.accdb");

}

QTEST_APPLESS_MAIN(lab)

#include "tst_lab.moc"
