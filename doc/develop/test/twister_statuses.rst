.. _twister_statuses:

Twister Status
##############

What is a Twister Status?
=========================

Twister Status formulates the current state of

- ``Harness``
- ``TestCase``
- ``TestSuite``
- ``TestInstance``

in a comprehensive and easy-to understand way.
In practice, most users will be interested in Statuses
of Instances and Cases after the conclusion of their Twister runs.

.. tip::

   Nomenclature reminder:

   .. tabs::

      .. tab:: ``Harness``

         ``Harness`` is a Python class inside Twister that allows us
         to capture and analyse output from a program external to Twister.

      .. tab:: ``TestCase``

         ``TestCase`` is a piece of code that aims to verify some assertion.
         It is the smallest subdivision of testing in Zephyr.

      .. tab:: ``TestSuite``

         ``TestSuite`` is a grouping of Test Cases.
         One can modify Twister's behaviour on a per-Suite basis via ``testcase.yaml`` files.
         Such grouped Cases should have enough in common
         for it to make sense to treat them all the same by Twister.

      .. tab:: ``TestInstance``

         ``TestInstance`` is a Test Suite on some platform.
         Twister reports typically report the results for Instances,
         despite them being called "Suites" there.
         As the distinction between them is not useful in this section,
         whenever you read "Suite", assume the same for Instances.

   More detailed explanation can be found :ref:`here <twister_tests_long_version>`.

Possible Twister Statuses
=========================

.. list-table:: Twister Statuses
   :widths: 10 10 65 5 10
   :header-rows: 1

   * - In-code
     - In-text
     - Description
     - Terminal
     - Where applicable
   * - NONE
     - None
     - No status. Starting point.
     - NO
     - Suite, Case
   * - STARTED
     - started
     - The test is currently in progress.
     - NO
     - Case
   * - NOTRUN
     - not run
     - The test wasn't run because it was not runnable in current configuration.
       It was, however, built correctly.
     - YES
     - Suite, Case
   * - BLOCK
     - blocked
     - The test was not run because of an error or crash in the test suite.
     - YES
     - Case
   * - FILTER
     - filtered
     - A static or runtime filter has eliminated the test from the list of tests to use.
     - YES
     - Suite, Case
   * - ERROR
     - error
     - The test produced an error in running the test itself.
     - YES
     - Suite, Case
   * - FAIL
     - failed
     - The test produced results different than expected.
     - YES
     - Suite, Case
   * - PASS
     - passed
     - The test produced results as expected.
     - YES
     - Suite, Case
   * - SKIP
     - skipped
     - Test was skipped by some other reason than previously delineated.
     - YES
     - Suite, Case

**In-code** and **In-text** columns should the names of a given status.
If one is not a Twister developer, the in-text name should suffice.
It is the name used in the JSON reports.

**Terminal** column indicates whether a given status should appear in the final Twister report.
Appearance of non-terminal Statuses in one's report files indicates a problem with Twister.

**Where applicable** column indicates where we might find such Statuses.
Harness has been omitted for clarity, as it does not appear in final report files.
If a status is marked as applicable for Suites, it is also applicable for Instances.


Test Case and Test Suite Status combinations
============================================

.. list-table:: Test Case and Test Suite Status combinations
   :widths: 16 12 12 12 12 12 12 12
   :align: center
   :header-rows: 1
   :stub-columns: 1

   * - Case\\Suite
     - NONE
     - FILTER
     - ERROR
     - FAIL
     - PASS
     - NOTRUN
     - SKIP
   * - NONE
     - N/A
     - N/A
     - N/A
     - N/A
     - N/A
     - N/A
     - N/A
   * - STARTED
     - N/A
     - N/A
     - N/A
     - N/A
     - N/A
     - N/A
     - N/A
   * - FILTER
     - N/A
     - ✓
     - ✕
     - ✕
     - ✕
     - ✕
     - ✕
   * - ERROR
     - N/A
     - ✕
     - ✓
     - ✕
     - ✕
     - ✕
     - ✕
   * - BLOCK
     - N/A
     - ✕
     - ✓
     - ✓
     - ✕
     - ✕
     - ✕
   * - FAIL
     - N/A
     - ✕
     - ✓
     - ✓
     - ✕
     - ✕
     - ✕
   * - PASS
     - N/A
     - ✕
     - ✓
     - ✓
     - ✓
     - ✕
     - ✕
   * - NOTRUN
     - N/A
     - ✕
     - ✓
     - ✓
     - ✓
     - ✓
     - ✕
   * - SKIP
     - N/A
     - ✕
     - ✓
     - ✓
     - ✓
     - ✓
     - ✓

✕ indicates that such a combination should not happen in a proper Twister run. In other words,
no Suite of a status indicated by the table column should contain any Cases of a status indicated
by the table row. ✓ indicates a proper combination.

Note that ``NONE`` and ``STARTED`` are not proper terminal statuses.
Their existence at the end of a Twister run indicates a problem with the Twister itself.

Detailed explanation, per Test Suite Status
-------------------------------------------

``NONE``:
  This is not a proper terminal status. It indicates an internal Twister problem. In proper
  operation, no Test Suite should get this status at the conclusion of a Twister run.

``FILTER``:
  This status indicates that the whole Suite has been statically filtered
  out of a given Twister run. Thus, any Case within it should also have such a status.

``ERROR``:
  Suite encountered a problem when running the test. It requires at least one case with
  ``ERROR`` or ``BLOCK`` status. As this takes precedence over all other Case statuses, all valid
  terminal Case statuses can be within such a Suite.

``FAIL``:
  Suite has at least one Case that did not meet its assertions. This takes precedence over
  all other Case statuses, given that the conditions for an ERROR status have not been met.

``PASS``:
  Suite has passed properly. It cannot contain any Cases with ``BLOCK``, ``ERROR``, or ``FAIL``
  statuses, as those indicate a problem when running the Suite.

``NOTRUN``:
  Whole suite was not run, but only built. It requires than all Cases within were not run.
  Thus, only ``NOTRUN`` and ``SKIP`` statuses are applicable for its Cases.

``SKIP``:
  Whole Suite has been skipped at runtime. All Cases need to have ``SKIP`` status as well.
