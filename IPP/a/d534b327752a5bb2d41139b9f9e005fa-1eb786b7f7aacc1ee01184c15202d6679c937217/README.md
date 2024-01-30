# How to set up

- you need the following in the folder with your source file:
  - `test.php` from this gist
  - `options` and `jexamxml.jar` from the `/pub/courses/ipp/jexamxml` folder on Merlin
  - `tests` folder with test cases (original set from Moodle or extended set from Discord pins) 
- run with `php8.1 test.php` on Merlin or with `php test.php` on your local machine

## Running individual tests

You can run individual tests like this: `php test.php header/ok`, or whole groups like this: `php test.php header`.