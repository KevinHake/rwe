import { Collapse, Divider, Paper, Typography } from "@material-ui/core";
import * as React from "react";
import { connect } from "react-redux";
import { Dispatch } from "redux";
import {
  joinSelectedGameCancel,
  joinSelectedGameConfirm,
  hostGame,
  joinSelectedGame,
  launchRwe,
  changeSinglePlayerMods,
} from "../actions";
import {
  State,
  canHostGame,
  canJoinSelectedGame,
  canLaunchRwe,
} from "../state";
import BottomPanel from "./BottomPanel";
import GamesTable from "./GamesTable";
import { PlayerNameDialog } from "./PlayerNameDialog";
import { Wizard } from "./Wizard";
import { next, close } from "../wizardActions";
import { WizardState } from "../wizard";
import { SelectModsDialog } from "./SelectModsDialog";

function MainPanel() {
  return (
    <div className="main-panel">
      <Typography variant="h6" className="main-panel-title">
        Online Games
      </Typography>
      <GamesTable />
    </div>
  );
}

interface ConnectionNoticeProps {
  connected: boolean;
}

function ConnectionNotice(props: ConnectionNoticeProps) {
  return (
    <React.Fragment>
      <Collapse in={!props.connected}>
        <div className="connection-notice">
          <Typography>
            Cannot reach the master server, attempting to reconnect...
          </Typography>
        </div>
      </Collapse>
      <Divider />
    </React.Fragment>
  );
}

interface OverviewScreenDispatchProps {
  onDialogClose: () => void;
  onDialogConfirm: (name: string) => void;
  onWizardNext: (path?: string) => void;
  onWizardClose: () => void;
  onHostGame: () => void;
  onJoinGame: () => void;
  onLaunchRwe: () => void;
  onChangeMods: (newMods: string[]) => void;
}

interface OverviewScreenStateProps {
  installedMods: string[];
  activeMods: string[];
  connected: boolean;
  dialogOpen: boolean;
  wizardState?: WizardState;
  hostEnabled: boolean;
  joinEnabled: boolean;
  launchEnabled: boolean;
}

interface OverviewScreenProps
  extends OverviewScreenDispatchProps,
    OverviewScreenStateProps {}

function OverviewScreen(props: OverviewScreenProps) {
  const [modsDialogOpen, setModsDialogOpen] = React.useState(false);
  return (
    <>
      <Paper className="app-container">
        <ConnectionNotice connected={props.connected} />
        <MainPanel />
        <BottomPanel
          hostEnabled={props.hostEnabled}
          joinEnabled={props.joinEnabled}
          launchEnabled={props.launchEnabled}
          onHostGame={props.onHostGame}
          onJoinGame={props.onJoinGame}
          onLaunchRwe={props.onLaunchRwe}
          onOpenModsDialog={() => setModsDialogOpen(true)}
        />
        <PlayerNameDialog
          open={props.dialogOpen}
          onConfirm={props.onDialogConfirm}
          onClose={props.onDialogClose}
        />
      </Paper>
      {props.wizardState && (
        <Wizard
          state={props.wizardState.step}
          onNext={props.onWizardNext}
          onClose={props.onWizardClose}
        />
      )}
      {modsDialogOpen && (
        <SelectModsDialog
          title="Select Single Player Mods"
          items={props.installedMods}
          initiallyActiveItems={props.activeMods}
          onSubmit={newMods => {
            setModsDialogOpen(false);
            props.onChangeMods(newMods);
          }}
          onCancel={() => setModsDialogOpen(false)}
        />
      )}
    </>
  );
}

function mapStateToProps(state: State): OverviewScreenStateProps {
  const dialogOpen =
    state.currentScreen.screen === "overview"
      ? state.currentScreen.dialogOpen
      : false;
  return {
    connected: state.masterServerConnectionStatus === "connected",
    dialogOpen,
    wizardState: state.wizard,
    installedMods: (state.installedMods || []).map(x => x.name),
    activeMods: state.activeMods,
    hostEnabled: canHostGame(state),
    joinEnabled: canJoinSelectedGame(state),
    launchEnabled: canLaunchRwe(state),
  };
}

function mapDispatchToProps(dispatch: Dispatch): OverviewScreenDispatchProps {
  return {
    onDialogClose: () => {
      dispatch(joinSelectedGameCancel());
    },
    onDialogConfirm: (name: string) => {
      dispatch(joinSelectedGameConfirm(name));
    },
    onWizardNext: (path?: string) => {
      dispatch(next(path));
    },
    onWizardClose: () => {
      dispatch(close());
    },
    onChangeMods: (mods: string[]) => {
      dispatch(changeSinglePlayerMods(mods));
    },
    onHostGame: () => dispatch(hostGame()),
    onJoinGame: () => dispatch(joinSelectedGame()),
    onLaunchRwe: () => dispatch(launchRwe()),
  };
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(OverviewScreen);
