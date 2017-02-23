
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
    public $project;
    public $environment;

    public function __construct($project, $environment)
    {
        $this->project = $project;
        $this->environment = $environment;
        
    }
    /**
     * @Given I am at project site
     */
    public function iAmAtProjectSite()
    {
        $this->visitPath("project/".$this->project);
    }

}