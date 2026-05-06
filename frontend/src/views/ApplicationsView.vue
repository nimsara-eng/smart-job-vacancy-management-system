<template>
  <div class="page">
    <div class="container">
      <div class="page-heading">
        <h1>Applications</h1>
        <!-- FIXED: Accessing 'auth.user' directly fixes the Ref unwrapping bug -->
        <p v-if="auth.user?.role === 'applicant'">Track your job applications</p>
        <p v-else>Review applications for your job listings</p>
      </div>

      <!-- Employer: filter by job -->
      <div v-if="auth.user?.role === 'employer'" class="filter-bar">
        <select v-model="filterJobId" class="form-select" style="max-width:280px">
          <option :value="null">All Jobs</option>
          <option v-for="job in myJobs" :key="job.id" :value="job.id">{{ job.title }}</option>
        </select>
        <div class="filter-chips">
          <button v-for="s in statuses" :key="s" class="filter-chip" :class="{ active: filterStatus === s }" @click="filterStatus = filterStatus === s ? '' : s">
            {{ s }}
          </button>
        </div>
      </div>

      <!-- Applicant: filter by status -->
      <div v-else class="filter-bar">
        <div class="filter-chips">
          <button class="filter-chip" :class="{ active: filterStatus === '' }" @click="filterStatus = ''">All</button>
          <button v-for="s in statuses" :key="s" class="filter-chip" :class="{ active: filterStatus === s }" @click="filterStatus = filterStatus === s ? '' : s">
            {{ s }}
          </button>
        </div>
      </div>

      <!-- Empty -->
      <div v-if="displayed.length === 0" class="empty-state">
        <svg width="48" height="48" viewBox="0 0 24 24" fill="none"><rect x="3" y="3" width="18" height="18" rx="2" stroke="currentColor" stroke-width="1.5"/><path d="M7 8h10M7 12h10M7 16h6" stroke="currentColor" stroke-width="1.5" stroke-linecap="round"/></svg>
        <p>No applications found.</p>
      </div>

      <!-- Table -->
      <div v-else class="apps-table card">
        <table>
          <thead>
            <tr>
              <th>Job</th>
              <th v-if="auth.user?.role === 'employer'">Applicant</th>
              <th>Applied Date</th>
              <th>Status</th>
              <th v-if="auth.user?.role === 'employer'">Actions</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="app in displayed" :key="app.applicationId">
              <td>
                <RouterLink :to="`/jobs/${app.jobId}`" class="app-job-link">
                  <strong>{{ jobTitle(app.jobId) }}</strong>
                  <span>{{ jobCompany(app.jobId) }}</span>
                </RouterLink>
              </td>
              <td v-if="auth.user?.role === 'employer'">
                <div class="app-user">
                  <div class="mini-avatar">{{ userInitials(app.userId) }}</div>
                  {{ userName(app.userId) }}
                </div>
              </td>
              <td>{{ app.appliedDate }}</td>
              <td>
                <span :class="statusBadge(app.status)">{{ app.status }}</span>
              </td>
              <td v-if="auth.user?.role === 'employer'">
                <div class="action-row">
                  <button
                    v-if="app.status !== 'approved'"
                    class="btn btn-success btn-sm"
                    @click="data.updateApplicationStatus(app.applicationId, 'approved')"
                  >Approve</button>
                  <button
                    v-if="app.status !== 'rejected'"
                    class="btn btn-danger btn-sm"
                    @click="data.updateApplicationStatus(app.applicationId, 'rejected')"
                  >Reject</button>
                </div>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { useRoute } from 'vue-router'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'

const auth = useAuthStore()
const data = useDataStore()
const route = useRoute()

const filterStatus = ref('' as string)
const filterJobId = ref<number | null>(route.query.job ? Number(route.query.job) : null)
const statuses = ['pending', 'approved', 'rejected']

// FIXED: Using `auth.user` and `data.jobs` directly. Pinia stores are fully reactive automatically.
const myJobs = computed(() =>
  auth.user?.role === 'employer'
    ? data.jobs.filter(j => j.company === auth.user?.name || j.company === auth.user?.username)
    : [],
)

const allApps = computed(() => {
  if (auth.user?.role === 'applicant') return data.getApplicationsByUser(auth.user.id)
  
  const jobIds = new Set(myJobs.value.map(j => j.id))
  return data.applications.filter(a => jobIds.has(a.jobId))
})

const displayed = computed(() => {
  let apps = allApps.value
  if (filterStatus.value) apps = apps.filter(a => a.status === filterStatus.value)
  if (auth.user?.role === 'employer' && filterJobId.value) {
    apps = apps.filter(a => a.jobId === filterJobId.value)
  }
  return apps
})

function jobTitle(id: number) { return data.getJobById(id)?.title ?? 'Unknown' }
function jobCompany(id: number) { return data.getJobById(id)?.company ?? '' }
function userName(id: number) { return data.users.find(u => u.id === id)?.name ?? 'Unknown' }
function userInitials(id: number) {
  return (data.users.find(u => u.id === id)?.name ?? '?').split(' ').map(w => w[0]).join('').toUpperCase().slice(0, 2)
}
function statusBadge(s: string) {
  return { pending: 'badge badge-amber', approved: 'badge badge-green', rejected: 'badge badge-red' }[s] ?? 'badge badge-gray'
}
</script>

<style scoped>
.filter-bar { display: flex; align-items: center; gap: 16px; flex-wrap: wrap; margin-bottom: 24px; }
.filter-chips { display: flex; gap: 6px; flex-wrap: wrap; }
.filter-chip {
  padding: 6px 14px;
  border-radius: 100px;
  border: 1px solid var(--c-border);
  background: transparent;
  color: var(--c-text2);
  font-size: .8rem;
  cursor: pointer;
  transition: all .15s;
  text-transform: capitalize;
}
.filter-chip.active, .filter-chip:hover { border-color: var(--c-accent); color: var(--c-accent); background: rgba(79,142,247,.08); }

.apps-table { overflow-x: auto; }
table { width: 100%; border-collapse: collapse; }
th { padding: 12px 16px; font-size: .75rem; font-weight: 500; color: var(--c-text3); text-align: left; text-transform: uppercase; letter-spacing: .04em; border-bottom: 1px solid var(--c-border); white-space: nowrap; }
td { padding: 14px 16px; font-size: .88rem; border-bottom: 1px solid var(--c-border); vertical-align: middle; }
tr:last-child td { border-bottom: none; }

.app-job-link { display: flex; flex-direction: column; gap: 1px; }
.app-job-link strong { font-weight: 500; transition: color .15s; }
.app-job-link:hover strong { color: var(--c-accent); }
.app-job-link span { font-size: .78rem; color: var(--c-text2); }

.app-user { display: flex; align-items: center; gap: 8px; }
.mini-avatar {
  width: 26px; height: 26px;
  border-radius: 50%;
  background: linear-gradient(135deg, rgba(79,142,247,.25), rgba(124,90,247,.25));
  display: grid; place-items: center;
  font-size: .68rem; font-weight: 700;
  color: var(--c-accent);
  flex-shrink: 0;
}
.action-row { display: flex; gap: 6px; }
</style>