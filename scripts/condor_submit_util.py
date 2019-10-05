#!/bin/env python

import time
import json
import metis

from time import sleep

from metis.Sample import DirectorySample, SNTSample
from metis.CondorTask import CondorTask

from metis.StatsParser import StatsParser

import sys
import os
import glob
import subprocess

hadoop_dirname = "wvzbaby"

#______________________________________________________________________________________
def get_tasks(samples_dictionary, year, baby_type, baby_version_tag, dotestrun=False):

    job_tag = "{}{}_{}".format(baby_type, year, baby_version_tag)

    # file/dir paths
    main_dir             = os.path.dirname(os.path.abspath(__file__))
    metis_path           = os.path.dirname(os.path.dirname(metis.__file__))
    tar_path             = os.path.join(metis_path, "package_{}.tar".format(job_tag))
    tar_gz_path          = tar_path + ".gz"
    exec_path            = os.path.join(main_dir, "metis.sh")
    merge_exec_path      = os.path.join(main_dir, "merge.sh")
    hadoop_path          = "metis/{}/{}".format(hadoop_dirname, job_tag) # The output goes to /hadoop/cms/store/user/$USER/"hadoop_path"

    # Extra arguments that will be passed on to ./processBaby to specific which baby to create from the babymaker binary executable
    args = ""
    if job_tag.find("WVZ") != -1:
        args = "1"
    if job_tag.find("Dilep") != -1:
        args = "2"
    if job_tag.find("Trilep") != -1:
        args = "3"
    if job_tag.find("WVZMVA") != -1:
        args = "4"
    if job_tag.find("Truth") != -1:
        args = "5"
    if job_tag.find("WVZAll") != -1:
        args = "6"

    # Change directory to metis
    os.chdir(metis_path)

    tasks = []

    # BEGIN Sample Loop -------->
    # loop over the samples
    for sample in sorted(samples_dictionary.iterkeys()):

        #
        # Job 1 : Creating baby
        #

        # define the task
        maker_task = CondorTask(
                sample               = SNTSample(dataset=sample,
                                                 # exclude_tag_pattern="CMS4_V08-*", # ignore new samples by sicheng for 2016 
                                                 exclude_tag_pattern="*v516*", # ignore new samples by sicheng for 2016 
                                                 ),
                tag                  = job_tag,
                arguments            = args,
                executable           = exec_path,
                tarfile              = tar_gz_path,
                special_dir          = hadoop_path,
                output_name          = "output.root",
                files_per_output     = 1,
                # files_per_output     = 1,
                condor_submit_params = {"sites" : "T2_US_UCSD"},
                # condor_submit_params = {"sites" : "UAF,T2_US_Wisconsin,T2_US_Florida,T2_US_Nebraska,T2_US_Caltech,T2_US_MIT,T2_US_Purdue"},
                # condor_submit_params = {"sites" : "UAF,T2_US_Wisconsin,T2_US_Florida,T2_US_Nebraska,T2_US_Caltech,T2_US_MIT"},
                # condor_submit_params = {"sites" : "UAF"},
                open_dataset         = False,
                flush                = True,
                max_jobs             = 5 if dotestrun else 0,
                # min_completion_fraction = 1.0 if "Run201" in sample else 0.9,
                # min_completion_fraction = 0.9,
                #no_load_from_backup  = True,
                )

        print sample, job_tag

        tasks.append(maker_task)

        #
        # Job 2 : Merging baby outputs
        #

        if maker_task.complete() and not dotestrun:

            merge_sample_name = "/MERGE_"+sample[1:]

            merge_task = CondorTask(
                    sample                 = DirectorySample(dataset=merge_sample_name, location=maker_task.get_outputdir()),
                    # open_dataset         = True, flush = True,
                    executable             = merge_exec_path,
                    tarfile                = tar_gz_path,
                    files_per_output       = 1,
                    output_dir             = maker_task.get_outputdir() + "/merged",
                    output_name            = samples_dictionary[sample] + ".root",
                    condor_submit_params   = {"sites":"UAF"},
                    output_is_tree         = True,
                    # check_expectedevents = True,
                    tag                    = job_tag,
                    cmssw_version          = "CMSSW_9_2_0",
                    scram_arch             = "slc6_amd64_gcc530",
                    #no_load_from_backup    = True,
                    max_jobs               = 1,
                    )
            merge_task.reset_io_mapping()
            merge_task.update_mapping()

            tasks.append(merge_task)

    # <------ END Sample Loop

    return tasks

#______________________________________________________________________________________
def create_tar_ball(year, baby_type, baby_version_tag):

    job_tag = "{}{}_{}".format(baby_type, year, baby_version_tag)

    # Create tarball
    metis_path = os.path.dirname(os.path.dirname(metis.__file__))
    tar_path = os.path.join(metis_path, "package_{}.tar".format(job_tag))
    tar_gz_path = tar_path + ".gz"
    main_dir = os.path.dirname(os.path.abspath(__file__))
    os.chdir(main_dir)
    exists = os.path.isfile(tar_gz_path)
    if not exists:
        os.system("tar -chJf {} ../setup.sh ../processBaby ../coreutil/data ../CORE/Tools/ ../rooutil/*.sh ../rooutil/hadd.py ../rooutil/addHistos.sh ../scale1fbs.txt ../StopAnalysis/StopCORE/METCorr/".format(tar_gz_path))

#______________________________________________________________________________________
def submit(dinfos, version_tag, dotestrun=False):

    # Loop
    while True:

        tasks = [] # List to hold all the Metis Tasks
        total_summary = {} # Summary for task status report to access via http

        # Boolean to aggregate all the task compelete status
        all_tasks_complete = True

        # Loop over all the campaigns
        for campaign in dinfos:

            create_tar_ball(dinfos[campaign]["year"], dinfos[campaign]["baby_type"], version_tag) # Create tar ball

            # Get all the tasks for this campaign
            this_set_of_tasks = get_tasks(
                    samples_dictionary = dinfos[campaign]["samples"],
                    year               = dinfos[campaign]["year"],
                    baby_type          = dinfos[campaign]["baby_type"],
                    baby_version_tag   = version_tag,
                    dotestrun          = dotestrun,
                    )

            # Need separate dictionaries for each tag in order to let StatsParsers uniquely handle same dataset name jobs.
            if len(this_set_of_tasks) > 0:
                print this_set_of_tasks[0].tag
                total_summary[this_set_of_tasks[0].tag] = {}

            # Add to the master list of tasks
            tasks += this_set_of_tasks

        # Loop over all the tasks
        for task in tasks:

            # Process the task (submit or resubmit)
            task.process()

            # save some information for the dashboard
            total_summary[task.tag][task.get_sample().get_datasetname()] = task.get_task_summary()

            # Aggregate task complete booleans
            all_tasks_complete = all_tasks_complete and task.complete()

        # parse the total summary and write out the dashboard
        #metis_dashboard_path = os.path.join(os.path.dirname(os.path.dirname(metis.__file__)), "dashboard")
        metis_dashboard_path = "~/public_html/dump"

        # Go through each tag and do StatsParser
        for tag in total_summary:
            StatsParser(data=total_summary[tag], webdir=metis_dashboard_path).do()

        # Print msummary table so I don't have to load up website
        #os.system("msummary -r -p MAKER | tee summary.txt")
        os.system("msummary -r | tee summary.txt")
        os.system("chmod -R 755 {}".format(metis_dashboard_path))

        # If all done exit the loop
        if all_tasks_complete:
            print ""
            print "All job finished"
            print ""
            break

        # Neat trick to not exit the script for force updating
        print 'Press Ctrl-C to force update, otherwise will sleep for 300 seconds'
        try:
            for i in range(0,300):
                sleep(1) # could use a backward counter to be preeety :)
        except KeyboardInterrupt:
            raw_input("Press Enter to force update, or Ctrl-C to quit.")
            print "Force updating..."

