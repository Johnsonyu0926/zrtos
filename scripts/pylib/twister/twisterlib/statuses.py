#!/usr/bin/env python3
# Copyright (c) 2024 Intel Corporation
# Copyright (c) 2024 NXP
#
# SPDX-License-Identifier: Apache-2.0
"""
Status classes to be used instead of str statuses.
"""
from __future__ import annotations

from colorama import Fore, init
from enum import Enum
from statemachine import StateMachine, State

from statemachine.contrib.diagram import DotGraphMachine
import pickle

class TwisterStatus(str, Enum):
    def __str__(self):
        return str(self.value)

    @classmethod
    def _missing_(cls, value):
        super()._missing_(value)
        if value is None:
            return TwisterStatus.NONE

    @staticmethod
    def get_color(status: TwisterStatus) -> str:
        status2color = {
            TwisterStatus.PASS: Fore.GREEN,
            TwisterStatus.NOTRUN: Fore.CYAN,
            TwisterStatus.SKIP: Fore.YELLOW,
            TwisterStatus.FILTER: Fore.YELLOW,
            TwisterStatus.BLOCK: Fore.YELLOW,
            TwisterStatus.FAIL: Fore.RED,
            TwisterStatus.ERROR: Fore.RED,
            TwisterStatus.STARTED: Fore.MAGENTA,
            TwisterStatus.NONE: Fore.MAGENTA
        }
        return status2color[status] if status in status2color else Fore.RESET

    # All statuses below this comment can be used for TestCase
    BLOCK = 'blocked'
    STARTED = 'started'

    # All statuses below this comment can be used for TestSuite
    # All statuses below this comment can be used for TestInstance
    FILTER = 'filtered'
    NOTRUN = 'not run'

    # All statuses below this comment can be used for Harness
    NONE = None
    ERROR = 'error'
    FAIL = 'failed'
    PASS = 'passed'
    SKIP = 'skipped'


class TwisterStatusMachine_inner(StateMachine):
    '''
        twister status statue machine
    '''
    none_statue = State(str(TwisterStatus.NONE), initial=True)
    pass_statue = State(str(TwisterStatus.PASS))
    notrun_statue = State(str(TwisterStatus.NOTRUN))
    skip_statue = State(str(TwisterStatus.SKIP))
    filter_statue = State(str(TwisterStatus.FILTER))
    block_statue = State(str(TwisterStatus.BLOCK))
    fail_statue = State(str(TwisterStatus.FAIL))
    error_statue = State(str(TwisterStatus.ERROR))
    started_statue = State(str(TwisterStatus.STARTED))

    # Define transitions
    cycles = (
        none_statue.to(started_statue,    cond="is_start")
        | none_statue.to(fail_statue,     cond="is_abnormal")
        | started_statue.to(fail_statue,  cond="is_abnormal")
        | started_statue.to(pass_statue,  cond="is_pass")
        | started_statue.to(skip_statue,  cond="is_skip")
        | started_statue.to(filter_statue,cond="is_filtered")
        | started_statue.to(block_statue, cond="is_blocked")
        | started_statue.to(error_statue, cond="is_error")
        | started_statue.to(notrun_statue,cond="is_build_only")
        | fail_statue.to(none_statue,    cond="is_force")
        | pass_statue.to(none_statue,    cond="is_force")
        | skip_statue.to(none_statue,    cond="is_force")
        | block_statue.to(none_statue,   cond="is_force")
        | error_statue.to(none_statue,   cond="is_force")
        | notrun_statue.to(none_statue,  cond="is_force")
        | filter_statue.to(none_statue,  cond="is_force")
        | none_statue.to.itself(internal=True,  cond="is_force")
        | started_statue.to.itself(internal=True)
        | fail_statue.to.itself(internal=True)
        | pass_statue.to.itself(internal=True)
        | skip_statue.to.itself(internal=True)
        | block_statue.to.itself(internal=True)
        | error_statue.to.itself(internal=True)
        | notrun_statue.to.itself(internal=True)
        | filter_statue.to.itself(internal=True)
    )


    def is_start(self, cond: TwisterStatus):
        if cond == TwisterStatus.STARTED:
            return True
        return False

    def is_abnormal(self, cond: TwisterStatus):
        if cond == TwisterStatus.FAIL:
            return True
        return False

    def is_pass(self, cond: TwisterStatus):
        if cond == TwisterStatus.PASS:
            return True
        return False

    def is_skip(self, cond: TwisterStatus):
        if cond == TwisterStatus.SKIP:
            return True
        return False

    def is_filtered(self, cond: TwisterStatus):
        if cond == TwisterStatus.FILTER:
            return True
        return False

    def is_blocked(self, cond: TwisterStatus):
        if cond == TwisterStatus.BLOCK:
            return True
        return False

    def is_error(self, cond: TwisterStatus):
        if cond == TwisterStatus.ERROR:
            return True
        return False

    def is_build_only(self, cond: TwisterStatus):
        if cond == TwisterStatus.NOTRUN:
            return True
        return False

    def is_force(self, cond: TwisterStatus):
        if cond == TwisterStatus.NONE:
            return True
        return False

    def after_transition(self, event: str, source: State, target: State, event_data):
        pass

    def before_build(self, event: str, source: State, target: State, message: str = ""):
        message = ". " + message if message else ""
        return f"Build: Switching {event} from {source.id} to {target.id}{message}"

    def before_run(self, event: str, source: State, target: State, message: str = ""):
        message = ". " + message if message else ""
        return f"Run: Switching {event} from {source.id} to {target.id}{message}"

    def trigger(self, event : TwisterStatus) -> None:
        '''
            trigger statue
        '''
        self.send("cycles", event)

    def force_state(self, event : TwisterStatus) -> None:
        '''
            trigger statue
        '''
        if event == TwisterStatus.NONE:
            self.send("cycles", event)
            return

        if self.current_state.id != "none_statue":
            self.send("cycles", TwisterStatus.NONE)
        self.send("cycles", TwisterStatus.STARTED)
        self.send("cycles", event)


class TwisterStatusMachine():
    def __init__(self, state=TwisterStatus.NONE):
        self.current_state = state

    class TransitionNotAllowed(Exception):
        pass

    def trigger(self, event : TwisterStatus) -> None:
        tsm = TwisterStatusMachine_inner()
        tsm.force_state(self.current_state)
        try:
            tsm.trigger(event)
        except StateMachine.TransitionNotAllowed:
            raise TwisterStatusMachine.TransitionNotAllowed
        self.current_state = TwisterStatus(str(tsm.current_state))

    def force_state(self, event : TwisterStatus) -> None:
        self.current_state = event

def get_graph(tm: TwisterStatusMachine_inner)-> None:
    '''
        draw graphic
    '''
    graph = DotGraphMachine(tm)
    dot = graph()
    dot.write_png("state.png")

def main():
    # Initialize the colorama library
    init()
    tm = TwisterStatusMachine()
    serialized = pickle.dumps(tm)
    deserialized = pickle.loads(serialized)
    assert deserialized
    #tsm = TwisterStatusMachine_inner()
    #get_graph(tsm)
    tm.trigger(TwisterStatus.NONE)
    print(tm.current_state)
    assert str(tm.current_state) == TwisterStatus.NONE
    tm.trigger(TwisterStatus.STARTED)
    assert str(tm.current_state) == TwisterStatus.STARTED
    tm.trigger(TwisterStatus.PASS)
    assert str(tm.current_state) == TwisterStatus.PASS
    tm.trigger(TwisterStatus.NONE)
    assert str(tm.current_state) == TwisterStatus.NONE
    tm.force_state(TwisterStatus.FILTER)
    assert str(tm.current_state) == TwisterStatus.FILTER
    tm.force_state(TwisterStatus.PASS)
    assert str(tm.current_state) == TwisterStatus.PASS
    tm.force_state(TwisterStatus.FAIL)
    assert str(tm.current_state) == TwisterStatus.FAIL
    print(str(tm.current_state))
    tm.trigger(TwisterStatus.NONE)
    assert str(tm.current_state) == TwisterStatus.NONE
    tm.force_state(TwisterStatus.PASS)
    assert str(tm.current_state) == TwisterStatus.PASS

if __name__ == "__main__":
    main()
