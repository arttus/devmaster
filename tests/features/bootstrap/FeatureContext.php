
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
    /* Click some text
    *
    * @When I click on the text :arg1
    */
    public function iClickOnTheText($arg1)
    {
        $session = $this->getSession();
        $element = $session->getPage()->find(
            'xpath',
            $session->getSelectorsHandler()->selectorToXpath('xpath', '*//*[text()="'. $arg1 .'"]')
        );
        if (null === $element) {
            throw new \InvalidArgumentException(sprintf('Cannot find text: "%s"', $arg1));
        }
 
        $element->click();
 
    }

    


}