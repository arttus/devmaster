Feature: Enironment Settings: General
In order to change General Settings of the Environment
As an administrator
I need to submit the form
@api
  Scenario: Lock database settings
    Given I am logged in as a user with the "administrator" role
    When I am at "project/qa"
    Then I click "Environment Settings"
    And I check "Lock Database"
    When I press the "Save" button
    Then I should see "Environment settings saved for dev in project qa"
