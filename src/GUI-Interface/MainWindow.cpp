#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title(GuiTitle);
  set_child(MainGrid);

  
  JobsFrame.set_label("Scheduled jobs");
  JobsFrame.set_child(JobsGrid);
  
  CmdBuildDb.set_label("BUILD DB");
  
  JobsDefineFrame.set_label("Jobs definition");
  JobsDefineFrame.set_child(JobsDefineGrid);
  LblJobName.set_label("Job name:");
  
  JobsDefineGrid.attach(LblJobName,0,0);
  JobsDefineGrid.attach(TxtJobName,1,0);
  JobsGrid.attach(CmdBuildDb, 0, 0);
  MainGrid.attach(JobsDefineFrame, 0, 0);
  MainGrid.attach(JobsFrame, 0, 1);


  
}
