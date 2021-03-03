#include <QtTest>

// add necessary includes here
#include <filesystem>
#include <fstream>

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
    auto path = std::filesystem::current_path();
    auto fileName = path.u8string() + "\\file.file";
    std::ofstream file(fileName);
    auto qstringPath = QString::fromUtf8(fileName.c_str());
    FileInfo fileInfo(qstringPath);


    QCOMPARE(qstringPath.replace('\\', '/'), fileInfo.getFullpath());
    QCOMPARE(0, fileInfo.getLength());
    QCOMPARE(fileInfo.isDir(), false);
    QCOMPARE(fileInfo.isFile(), true);
    QCOMPARE(fileInfo.isSymLink(), false);
}

QTEST_APPLESS_MAIN(lab)

#include "tst_lab.moc"
