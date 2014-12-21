function varargout = main_inputfile(varargin)
% Task(menu).name   - name of task. Task(1).name is 'File', Task(2).name is
%                     'Model', Task(8).name is 'Help'.
% Task(menu).string - For each menu item (or any sub-items), there are 4
%                     parameters that needed to be specified.
%                     (1) index -- There are up to 3 cascading levels of
%                     menu items. Each level can have up to 9 menu items.
%                     For example, "100" represents menu item 1 of level 1.
%                     "236" is item 2 selected from level 1 and item 3 from
%                     level 2 and item 6 from level 3.
%                     (2) label -- menu item label corresponding to index
%                     (3) type  -- menu item type; type 'doc1' if the
%                     action is to open html or pdf file; 'run1' to execute
%                     a matlab function; etc.
%                     (4) operands -- for type 'doc1', this would be the
%                     html or pdf file; for type 'run1', this would be the
%                     name of the function and its arguments. 
%                     You can enter the menu items in any order
clear all 
if nargin > 0
% update gui4gui_inputfile with modified "Task" from the gui4gui input GUI
    GID = varargin{1};
    % insert any additional modifications here
    save gui4gui_inputfile GID;
else
% the gui4gui package comes with this mfile pre-loaded with the setup
% for the MODE code. Some of these data are universal for all codes if 
% you prepare PDF/HTML files and label them with the same names, such as
% to call the tutorial file Tutorial.ppt or name the abstract Abstract.pdf. 
% You may also edit this file and change any contents that you wish. 
% However, you should not remove any fields in Task. If you don't need
% a specific menu item, just remove its content, like this
% Task(1).label = {};
%
% Define number of menus (File, Model, Articles, Tutorial, Examples, Run, Code, Help)
Nmenus = 8;

Task(1).name = 'File';
Task(1).string = { ...
 100 'Exit'  'run2'  'exit' ...
};

Task(2).name = 'Model';
Task(2).string = { ...
 100 'System diagram'  'doc1'  'System_diagram.pdf' ...
%  200 'Input'  'doc1'  'Input.pdf' ...
%  300 'Non-directional transient cells'  'doc1'  'Non_directional_transient_cells.pdf' ...
%  400 'Directional transient cells'  'doc1'  'Directional_transient_cells.pdf' ...
%  500 'Directional short range filters'  'doc1'  'Directional short range filters.pdf' ...
%  600 'Spatial and opponent directional competition'  'doc1'  'Spatial_and_opponent_directional_competition.pdf' ...
%  700 'Directional long-range filters'  'doc1'  'Directional_long_range_filters.pdf' ...
%  800 'Directional grouping network'  'doc1'  'Directional_grouping_network.pdf' ...
%  900 'Decision cells and decision gating'  'doc1'  'Decision_cells_and_decision_gating.pdf' ...
};

Task(3).name = 'Articles';
Task(3).string = { ...
 100 'Izhikevich (2003)'  'doc1'  'Izh_spikes.pdf' ...
};

Task(4).name = 'Tutorial';
Task(4).string = { ...
 100 'Abstract'  'doc1'  'Abstract.pdf' ...
 200 'Tutorial'  'doc1'  'Tutorial.ppt' ...
};

Task(5).name = 'Examples';
Task(5).string = { ...
 100 'Preset image 1: Gail' 'run1' 'reducedSpikingDemo(1)' ...
 200 'Preset image 2: Wedding' 'run1' 'reducedSpikingDemo(2)' ...
 300 'Preset image 3: Dog' 'run1' 'reducedSpikingDemo(3)' ...
 400 'Preset image 4: Optical illusion1' 'run1' 'reducedSpikingDemo(4)' ...
 500 'Preset image 5: Optical illusion2' 'run1' 'reducedSpikingDemo(5)' ...
 600 'Preset image 6: Robert' 'run1' 'reducedSpikingDemo(6)' ...
% % %  120 'Non-directional transient cells'  'doc1'  'lowCoh_non-directional_transient_cells.html' ...
% % %  130 'Directional transient cells'  'doc1'  'lowCoh_directional_transient_cells.html' ...
% % %  140 'Directional short range filters'  'doc1'  'lowCoh_directional_short_range_filters.html' ...
% % %  150 'Directional long-range filters'  'doc1'  'lowCoh_directional_long-range_filters.html' ...
% % %  160 'Directional grouping network'  'doc1'  'lowCoh_directional_grouping_network.html' ...
% % %  170 'Decision cells and decision gating'  ''  '' ...
% % %  171 'Reaction time task'  'doc1'  'lowCoh_RT_task.html' ...
% % %  172 'Fixed duration task'  'doc1'  'lowCoh_FD_task.html' ...
% % %  200 'Medium coherence (6.4%)'  ''  '' ...
% % %  210 'Input'  'doc1'  'medCoh_input.html' ...
% % %  220 'Non-directional transient cells'  'doc1'  'medCoh_non-directional_transient_cells.html' ...
% % %  230 'Directional transient cells'  'doc1'  'medCoh_directional_transient_cells.html' ...
% % %  240 'Directional short range filters'  'doc1'  'medCoh_directional_short_range_filters.html' ...
% % %  250 'Directional long-range filters'  'doc1'  'medCoh_directional_long-range_filters.html' ...
% % %  260 'Directional grouping network'  'doc1'  'medCoh_directional_grouping_network.html' ...
% % %  270 'Decision cells and decision gating'  ''  '' ...
% % %  271 'Reaction time task'  'doc1'  'medCoh_RT_task.html' ...
% % %  272 'Fixed duration task'  'doc1'  'medCoh_FD_task.html' ...
% % %  300 'High coherence (51.2%)'  ''  '' ...
% % %  310 'Input'  'doc1'  'highCoh_input.html' ...
% % %  320 'Non-directional transient cells'  'doc1'  'highCoh_non-directional_transient_cells.html' ...
% % %  330 'Directional transient cells'  'doc1'  'highCoh_directional_transient_cells.html' ...
% % %  340 'Directional short range filters'  'doc1'  'highCoh_directional_short_range_filters.html' ...
% % %  350 'Directional long-range filters'  'doc1'  'highCoh_directional_long-range_filters.html' ...
% % %  360 'Directional grouping network'  'doc1'  'highCoh_directional_grouping_network.html' ...
% % %  370 'Decision cells and decision gating'  ''  '' ...
% % %  371 'Reaction time task'  'doc1'  'highCoh_RT_task.html' ...
% % %  372 'Fixed duration task'  'doc1'  'highCoh_FD_task.html' ...
};

Task(6).name = 'Run';
Task(6).string = { ...
    100 'Load an image and spike!' 'run1' 'reducedSpikingDemo(0)' ...
%  100 'Reaction time task'  ''  '' ...
%  110 'Description'  'doc1'  'RTtask_description.pdf' ...
%  120 'Replication test'  ''  '' ...
%  121 'Description'  'doc1'  'RTtask_description.pdf' ...
%  122 'Run'  'run1'  'RTtasknew(1,1,5,5,5,55);' ...
%  130 'Advanced run'  ''  '' ...
%  131 'Description'  'doc1'  'RTtask_description.pdf' ...
%  132 'Run'  'run1'  'Reaction_time_task_GUI' ...
%  200 'Fixed duration task'  ''  '' ...
%  210 'Description'  'doc1'  'FDtask_description.pdf' ...
%  220 'Replication test'  ''  '' ...
%  221 'Description'  'doc1'  'FDtask_description.pdf' ...
%  222 'Run'  'run1'  'FDtasknew(4.5,2,5,5,5,9);' ...
%  230 'Advanced run'  ''  '' ...
%  231 'Description'  'doc1'  'FDtask_description.pdf' ...
%  232 'Run'  'run1'  'Fixed_duration_task_GUI' ...
};

Task(7).name = 'Code';
Task(7).string = { ...
 100 'Source code'  'doc1'  'HTML/Index.html' ...
};

Task(8).name = 'Help';
Task(8).string = { ...
 100 'Contact'  'doc1'  'Contact.html' ...
 200 'Credit'  'doc1'  'Credit.html' ...
 300 'License'  'doc1'  'License.html' ...
};

for menu=1:Nmenus
    task(menu).name = Task(menu).name;
    for n=1:length(Task(menu).string(:))/4
        task(menu).string(n,1) = Task(menu).string(1+(n-1)*4);
        task(menu).string(n,2) = Task(menu).string(2+(n-1)*4);
        task(menu).string(n,3) = Task(menu).string(3+(n-1)*4);
        task(menu).string(n,4) = Task(menu).string(4+(n-1)*4);
    end
end
  GID.guiName = 'SpikingNeurons';
  GID.Nmenus = Nmenus;
  GID.frontImage = 'izhik.gif';
  GID.bgcolor = 'white';
  GID.fontsize = 10;
  GID.fontweight = 'bold';
  GID.position = [0.1 0.1 0.6 0.6];
  GID.resize = 'off';
  GID.task = task;
  
% The buildMainGUI builder needs fields label, type and ops
% The addLabel function can extract them from GID's string field
  GID = addLabel(GID);  
  
% returns the final GID to the caller.
  varargout{1} = GID;
% Also save GID to the following file.  
  save main_inputfile_tmp GID;
end   % if
