#include "./MainWindow.hpp"

MainWindow::MainWindow(){
  set_title(GuiTitle);
  set_child(MainGrid);

    
  JobsFrame.set_label("Scheduled jobs");
  JobsFrame.set_child(JobsGrid);
  
  CmdBuildDb.set_label("BUILD DB");
  CmdAddJob.set_label("Add job to list");
  CmdAddJob.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::AddJob2List));
  CmdDeleteJob.set_label("Remove job from list");
  CmdEditJob.set_label("Edit selected job");
  
  ListJobList.set_editable(false);
  ListJobList.set_wrap_mode(Gtk::WrapMode::WORD);
  ListJobList.set_size_request(-1, 400);

  JobsDefineFrame.set_label("Job definition");
  JobsDefineFrame.set_child(JobsDefineGrid);
  FrameJobName.set_label("Job name:");
  FrameJobName.set_child(TxtJobName);
  FrameDbName.set_label("Database name:");
  FrameDbName.set_child(TxtDbName);
  FrameIcdVers.set_label("ICD version");
  FrameIcdVers.set_child(ListIcdVers);
  FrameCountryList.set_label("Selected coutries");
  FrameCountryList.set_child(TxtCountryList);
  FrameSrcDir.set_label("Source directory");
  FrameSrcDir.set_child(TxtSrcDir);
  FrameAiracCycle.set_label("AIRAC cycle");
  FrameAiracCycle.set_child(TxtAiracCycle);
  FrameEndiannes.set_label("Endianness");
  FrameEndiannes.set_child(ListEndianness);

  // TxtDbName.property_text_length(true);
  // TxtAiracCycle.set_propagate_text_width(false);

  JobsDefineGrid.attach(FrameJobName,0,0);
  JobsDefineGrid.attach(FrameDbName,1,0);
  JobsDefineGrid.attach(FrameIcdVers,0,1);
  JobsDefineGrid.attach(FrameCountryList,1,1);
  JobsDefineGrid.attach(FrameSrcDir,0,2);
  JobsDefineGrid.attach(FrameAiracCycle,1,2);
  JobsDefineGrid.attach(FrameEndiannes,0,3);

  JobsGrid.attach(CmdAddJob, 0, 0);
  JobsGrid.attach(CmdEditJob, 1, 0);
  JobsGrid.attach(CmdDeleteJob, 2, 0);
  JobsGrid.attach(CmdBuildDb, 3, 0);
  ScrollerJobList.set_child(ListJobList);
  ScrollerJobList.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
  JobsGrid.attach(ScrollerJobList, 0, 1, 4, 3);

  // JobsGrid.attach(ListJobList, 1,1);
  MainGrid.attach(JobsDefineFrame, 0, 0);
  MainGrid.attach(JobsFrame, 0, 1);

  // show_all_childred();

}

void MainWindow::AddJob2List(){
  std::string jobName, dbName, srcDir;
  std::string ICD = "1";
  std::string JobList_string;
  std::list<GenerationJob>::iterator it = this->JobList.begin();
  jobName = this->TxtJobName.get_text();
  dbName = this->TxtDbName.get_text();
  srcDir = this->TxtSrcDir.get_text();
  this->JobList.push_back(GenerationJob(jobName, dbName, srcDir));
  for (it = this->JobList.begin(); it != this->JobList.end(); it++){
    JobList_string.append(it->ToString() + "\n");
  }
  this->ListJobList.get_buffer()->set_text(JobList_string);
  return;
}
