@action.platform_start.io_tree.json
Feature: API Io

    Panduza provides a way to control simple input/output signals

    Rule: API Io must be able to drive io direction

        Two topics are defined for this purpose:

        | Suffix                                | QOS | Retain |
        |:-------------------------------------:|:---:|:------:|
        | {INTERFACE_PREFIX}/atts/direction     | 0   | true   |
        | {INTERFACE_PREFIX}/cmds/direction/set | 0   | false  |

        The payload of those topics must be a json payload:

        | Key       | Type   | Description                       |
        |:-------- :|:------:|:---------------------------------:|
        | direction | string | direction of the io 'in' or 'out' |

        ```json
            {
                "direction": "in"
            }
        ```


        @fixture.interface.io.test
        Scenario: Io direction must be configurable
            Given core aliases loaded with file "io_fake_alias.json"
            And  io interface "test" initialized with alias "io_test"
            When io interface "test" direction is set to "out"
            Then io interface "test" direction is "out"
            When io interface "test" direction is set to "in"
            Then io interface "test" direction is "in"

        @fixture.interface.io.test
        Scenario: Io value must be configurable
            Given core aliases loaded with file "io_fake_alias.json"
            And  io interface "test" initialized with alias "io_test"
            When io interface "test" direction is set to "out"
            Then io interface "test" direction is "out"
            When io interface "test" value is set to "0"
            Then io interface "test" value is "0"

# -----------------------------------------------------------------------------

    @fixture.interface.io.<IO>
    Scenario Outline: Io value must support operation set and get through 2 interfaces in loopback
        Given core aliases loaded with file "io_alias.json"
        And io interface "<IO>" initialized with alias "<IO>"
        When io interface "<IO>" direction is set to "out"
        Then io interface "<IO>" direction is "out"
        When io interface "<IO>" value is set to "1"
        Then io interface "<IO>" value is "1"
        When io interface "<IO>" value is set to "0"
        Then io interface "<IO>" value is "0"

        Examples: IOs
            | IO            |
            | IO_E18        |
            | IO_F13        |
            | IO_E13        |
            | IO_H15        |
            | IO_J15        |
            | IO_G17        |
            | IO_F15        |
            | IO_E15        |
            | IO_F18        |
            | IO_E14        |
