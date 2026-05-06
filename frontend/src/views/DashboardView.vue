<template>
  <div class="page">
    <div class="container">
      <div class="dash-welcome">
        <h1>Good {{ greeting }}, <span class="name-highlight">{{ auth.user?.name }}</span> 👋</h1>
        <p>Here's your overview.</p>
      </div>

      <!-- Stats row -->
      <div class="stats-row">
        <div v-for="stat in stats" :key="stat.label" class="stat-card card">
          <div class="stat-icon" :style="{ background: stat.bg }">
            <span v-html="stat.icon"/>
          </div>
          <div>
            <div class="stat-value">{{ stat.value }}</div>
            <div class="stat-label">{{ stat.label }}</div>
          </div>
        </div>
      </div>

      <!-- Applicant sections -->
      <template v-if="auth.user?.role === 'applicant'">
        <!-- Matched jobs -->
        <section v-if="matchedJobs.length">
          <div class="section-header">
            <h2>Recommended for You</h2>
            <RouterLink to="/jobs" class="btn btn-ghost btn-sm">View all →</RouterLink>
          </div>
          <div class="grid-2">
            <JobCard
              v-for="m in matchedJobs.slice(0, 4)"
              :key="m.job.id"
              :job="m.job"
              :userSkills="auth.user?.skills ?? []"
              :matchScore="m.matchScore"
            />
          </div>
        </section>

        <!-- Recent applications -->
        <section v-if="myApplications.length" style="margin-top:32px">
          <div class="section-header">
            <h2>My Applications</h2>
            <RouterLink to="/applications" class="btn btn-ghost btn-sm">View all →</RouterLink>
          </div>
          <div class="apps-list">
            <div v-for="app in myApplications.slice(0, 5)" :key="app.applicationId" class="app-row card">
              <div class="app-info">
                <RouterLink :to="`/jobs/${app.jobId}`" class="app-title">
                  {{ jobTitle(app.jobId) }}
                </RouterLink>
                <span class="app-company">{{ jobCompany(app.jobId) }}</span>
              </div>
              <div class="app-right">
                <span :class="statusBadge(app.status)">{{ app.status }}</span>
                <span class="app-date">{{ app.appliedDate }}</span>
              </div>
            </div>
          </div>
        </section>

        <div v-if="!matchedJobs.length && !myApplications.length" class="empty-state">
          <svg width="48" height="48" viewBox="0 0 24 24" fill="none"><path d="M20 7H4C2.9 7 2 7.9 2 9v12c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V9c0-1.1-.9-2-2-2z" stroke="currentColor" stroke-width="1.5"/><path d="M16 7V5c0-1.1-.9-2-2-2h-4c-1.1 0-2 .9-2 2v2" stroke="currentColor" stroke-width="1.5"/></svg>
          <p>Update your profile skills to get job recommendations.</p>
          <RouterLink to="/profile" class="btn btn-primary">Update Profile</RouterLink>
        </div>
      </template>

      <!-- Employer sections -->
      <template v-else-if="auth.user?.role === 'employer'">
        <section>
          <div class="section-header">
            <h2>My Job Listings</h2>
            <RouterLink to="/post-job" class="btn btn-primary btn-sm">+ Post Job</RouterLink>
          </div>
          <div v-if="myJobs.length === 0" class="empty-state">
            <p>You haven't posted any jobs yet.</p>
            <RouterLink to="/post-job" class="btn btn-primary">Post Your First Job</RouterLink>
          </div>
          <div v-else class="jobs-table card">
            <table>
              <thead>
                <tr>
                  <th>Title</th>
                  <th>Category</th>
                  <th>Salary</th>
                  <th>Applications</th>
                  <th>Status</th>
                  <th>Actions</th>
                </tr>
              </thead>
              <tbody>
                <tr v-for="job in myJobs" :key="job.id">
                  <td><RouterLink :to="`/jobs/${job.id}`">{{ job.title }}</RouterLink></td>
                  <td><span class="badge badge-purple">{{ job.category }}</span></td>
                  <td>LKR {{ job.salary.toLocaleString() }}</td>
                  <td>{{ appCountForJob(job.id) }}</td>
                  <td>
                    <span :class="job.isOpen ? 'badge badge-green' : 'badge badge-red'">
                      {{ job.isOpen ? 'Open' : 'Closed' }}
                    </span>
                  </td>
                  <td class="table-actions">
                    <button class="btn btn-ghost btn-sm" @click="data.toggleJobStatus(job.id)">
                      {{ job.isOpen ? 'Close' : 'Reopen' }}
                    </button>
                    <RouterLink :to="`/applications?job=${job.id}`" class="btn btn-outline btn-sm">
                      Applications
                    </RouterLink>
                  </td>
                </tr>
              </tbody>
            </table>
          </div>
        </section>
      </template>
    </div>
  </div>
</template>

<script setup lang="ts">
import { computed } from 'vue'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'
import JobCard from '../components/JobCard.vue'

const auth = useAuthStore()
const data = useDataStore()


const greeting = computed(() => {
  const h = new Date().getHours()
  if (h < 12) return 'morning'
  if (h < 17) return 'afternoon'
  return 'evening'
})

const myApplications = computed(() =>
  auth.user ? data.getApplicationsByUser(auth.user.id) : [],
)
const matchedJobs = computed(() =>
  auth.user ? data.getMatchingJobsForUser(auth.user.skills) : [],
)
const myJobs = computed(() =>
  auth.user?.role === 'employer'
    ? data.jobs.filter(j => j.company === auth.user?.name || j.company === auth.user?.username)
    : [],
)

const stats = computed(() => {
  if (auth.user?.role === 'applicant') {
    return [
      { label: 'Total Applied', value: myApplications.value.length, bg: 'rgba(79,142,247,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><rect x="3" y="3" width="18" height="18" rx="2" stroke="#4f8ef7" stroke-width="1.8"/><path d="M7 8h10M7 12h10M7 16h6" stroke="#4f8ef7" stroke-width="1.8" stroke-linecap="round"/></svg>' },
      { label: 'Pending', value: myApplications.value.filter(a => a.status === 'pending').length, bg: 'rgba(247,169,64,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><circle cx="12" cy="12" r="9" stroke="#f7a940" stroke-width="1.8"/><path d="M12 7v5l3 3" stroke="#f7a940" stroke-width="1.8" stroke-linecap="round"/></svg>' },
      { label: 'Approved', value: myApplications.value.filter(a => a.status === 'approved').length, bg: 'rgba(62,207,142,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><path d="M5 12l5 5L20 7" stroke="#3ecf8e" stroke-width="2" stroke-linecap="round"/></svg>' },
      { label: 'Job Matches', value: matchedJobs.value.length, bg: 'rgba(124,90,247,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><path d="M12 2l3.09 6.26L22 9.27l-5 4.87 1.18 6.88L12 17.77l-6.18 3.25L7 14.14 2 9.27l6.91-1.01L12 2z" fill="#7c5af7"/></svg>' },
    ]
  }
  return [
    { label: 'Jobs Posted', value: myJobs.value.length, bg: 'rgba(79,142,247,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><rect x="2" y="7" width="20" height="14" rx="2" stroke="#4f8ef7" stroke-width="1.8"/><path d="M16 7V5a2 2 0 00-2-2h-4a2 2 0 00-2 2v2" stroke="#4f8ef7" stroke-width="1.8"/></svg>' },
    { label: 'Open Roles', value: myJobs.value.filter(j => j.isOpen).length, bg: 'rgba(62,207,142,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><circle cx="12" cy="12" r="9" stroke="#3ecf8e" stroke-width="1.8"/><path d="M8 12l3 3 5-5" stroke="#3ecf8e" stroke-width="1.8" stroke-linecap="round"/></svg>' },
    { label: 'Total Applications', value: myJobs.value.reduce((s, j) => s + appCountForJob(j.id), 0), bg: 'rgba(247,169,64,.12)', icon: '<svg width="20" height="20" viewBox="0 0 24 24" fill="none"><path d="M17 21v-2a4 4 0 00-4-4H5a4 4 0 00-4 4v2" stroke="#f7a940" stroke-width="1.8"/><circle cx="9" cy="7" r="4" stroke="#f7a940" stroke-width="1.8"/><path d="M23 21v-2a4 4 0 00-3-3.87M16 3.13a4 4 0 010 7.75" stroke="#f7a940" stroke-width="1.8"/></svg>' },
  ]
})

function jobTitle(jobId: number) { return data.getJobById(jobId)?.title ?? 'Unknown' }
function jobCompany(jobId: number) { return data.getJobById(jobId)?.company ?? '' }
function appCountForJob(jobId: number) { return data.getApplicationsByJob(jobId).length }
function statusBadge(status: string) {
  return { pending: 'badge badge-amber', approved: 'badge badge-green', rejected: 'badge badge-red' }[status] ?? 'badge badge-gray'
}
</script>

<style scoped>
.dash-welcome { margin-bottom: 32px; }
.dash-welcome h1 { font-size: clamp(1.4rem, 3vw, 2rem); margin-bottom: 4px; }
.dash-welcome p { color: var(--c-text2); }
.name-highlight {
  background: linear-gradient(135deg, var(--c-accent), var(--c-accent2));
  -webkit-background-clip: text;
  background-clip: text; /* Fixed compatibility warning */
  -webkit-text-fill-color: transparent;
}

.stats-row { display: grid; grid-template-columns: repeat(auto-fill, minmax(200px, 1fr)); gap: 16px; margin-bottom: 40px; }
.stat-card { padding: 20px; display: flex; align-items: center; gap: 16px; }
.stat-icon { width: 44px; height: 44px; border-radius: 10px; display: grid; place-items: center; flex-shrink: 0; }
.stat-value { font-family: var(--font-display); font-size: 1.8rem; font-weight: 700; line-height: 1; }
.stat-label { font-size: .8rem; color: var(--c-text2); margin-top: 4px; }

.section-header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 16px; }
.section-header h2 { font-size: 1.1rem; }

.apps-list { display: flex; flex-direction: column; gap: 8px; }
.app-row { padding: 14px 20px; display: flex; align-items: center; justify-content: space-between; gap: 16px; flex-wrap: wrap; }
.app-info { display: flex; flex-direction: column; gap: 2px; }
.app-title { font-weight: 500; font-size: .9rem; transition: color .15s; }
.app-title:hover { color: var(--c-accent); }
.app-company { font-size: .8rem; color: var(--c-text2); }
.app-right { display: flex; align-items: center; gap: 12px; }
.app-date { font-size: .78rem; color: var(--c-text3); }

.jobs-table { overflow-x: auto; }
table { width: 100%; border-collapse: collapse; }
th { padding: 12px 16px; font-size: .75rem; font-weight: 500; color: var(--c-text3); text-align: left; text-transform: uppercase; letter-spacing: .04em; border-bottom: 1px solid var(--c-border); }
td { padding: 14px 16px; font-size: .88rem; border-bottom: 1px solid var(--c-border); }
tr:last-child td { border-bottom: none; }
td a { font-weight: 500; transition: color .15s; }
td a:hover { color: var(--c-accent); }
.table-actions { display: flex; gap: 6px; }
</style>