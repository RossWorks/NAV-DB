#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title(GuiTitle);
  set_child(MainGrid);

  
  JobsFrame.set_label("Scheduled jobs");
  JobsFrame.set_child(JobsGrid);
  
  CmdBuildDb.set_label("BUILD DB");
  CmdAddJob.set_label("Add job to list");
  CmdDeleteJob.set_label("Remove job from list");
  CmdEditJob.set_label("Edit selected job");
  
  JobsDefineFrame.set_label("Jobs definition");
  JobsDefineFrame.set_child(JobsDefineGrid);
  LblJobName.set_label("Job name:");
  LblDbName.set_label("Database name:");
  LblIcdVers.set_label("ICD version");
  LblIcdVers.set_label("ICD version");
  LblCountryList.set_label("Selected coutries");
  LblSrcDir.set_label("Source directory");
  LblAiracCycle.set_label("AIRAC cycle");
  LblEndiannes.set_label("Endiannes");
  
  JobsDefineGrid.attach(LblJobName,0,0);
  JobsDefineGrid.attach(TxtJobName,1,0);
  JobsDefineGrid.attach(LblDbName,0,1);
  JobsDefineGrid.attach(TxtDbName,1,1);
  JobsDefineGrid.attach(LblIcdVers,0,2);
  JobsDefineGrid.attach(ListIcdVers,1,2);
  JobsDefineGrid.attach(LblCountryList,0,3);
  JobsDefineGrid.attach(TxtCountryList,1,3);
  JobsDefineGrid.attach(LblSrcDir,0,4);
  JobsDefineGrid.attach(TxtSrcDir,1,4);
  JobsDefineGrid.attach(LblAiracCycle,0,5);
  JobsDefineGrid.attach(TxtAiracCycle,1,5);
  JobsDefineGrid.attach(LblEndiannes,0,6);
  JobsDefineGrid.attach(ListEndianness,1,6);

  JobsGrid.attach(CmdAddJob, 0, 0);
  JobsGrid.attach(CmdEditJob, 1, 0);
  JobsGrid.attach(CmdDeleteJob, 2, 0);
  JobsGrid.attach(CmdBuildDb, 3, 0);
  MainGrid.attach(JobsDefineFrame, 0, 0);
  MainGrid.attach(JobsFrame, 0, 1);


  
}
