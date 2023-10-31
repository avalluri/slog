#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include "log_level_test.h"
#include "file_target_test.h"
#include "logger_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(LogLevelTest);
CPPUNIT_TEST_SUITE_REGISTRATION(FileTargetTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTest);

int main() {
    CPPUNIT_NS::TestResult testresult;
    
    CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener (&collectedresults);

    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    return collectedresults.wasSuccessful() ? 0 : 1;
}