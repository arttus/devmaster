
<?php

use Drupal\DrupalExtension\Context\RawDrupalContext;
use Behat\Behat\Context\SnippetAcceptingContext;
use Behat\Gherkin\Node\PyStringNode;
use Behat\Gherkin\Node\TableNode;


/**
 * Defines application features from the specific context.
 */
class FeatureContext extends RawDrupalContext implements SnippetAcceptingContext {
    private $output;

    public function __construct()
    {
        
    }
    /**
     * @Then the database is locked
     */
    public function lockDatabase()
    {
        print 'Lock Database';
    }
}