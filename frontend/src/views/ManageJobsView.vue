<template>
  <div class="page">
    <div class="container">
      <div class="header-row">
        <div class="page-heading" style="margin-bottom:0">
          <h1>Manage Jobs</h1>
          <p>All your posted listings</p>
        </div>
        <RouterLink to="/post-job" class="btn btn-primary">+ Post New Job</RouterLink>
      </div>

      <div v-if="myJobs.length === 0" class="empty-state">
        <svg width="48" height="48" viewBox="0 0 24 24" fill="none"><rect x="2" y="7" width="20" height="14" rx="2" stroke="currentColor" stroke-width="1.5"/><path d="M16 7V5a2 2 0 00-2-2h-4a2 2 0 00-2 2v2" stroke="currentColor" stroke-width="1.5"/></svg>
        <p>You haven't posted any jobs yet.</p>
        <RouterLink to="/post-job" class="btn btn-primary">Post Your First Job</RouterLink>
      </div>

      <div v-else class="jobs-grid grid-2">
        <div v-for="job in myJobs" :key="job.id" class="manage-card card">
          <!-- Header -->
          <div class="mc-header">
            <div>
              <h3 class="mc-title">{{ job.title }}</h3>
              <p class="mc-company">{{ job.company }}</p>
            </div>
            <div class="mc-status-badge" :class="job.isOpen ? 'open' : 'closed'">
              {{ job.isOpen ? 'Open' : 'Closed' }}
            </div>
          </div>

          <!-- Meta -->
          <div class="mc-meta">
            <span><svg width="12" height="12" viewBox="0 0 24 24" fill="none"><path d="M12 2C8.13 2 5 5.13 5 9c0 5.25 7 13 7 13s7-7.75 7-13c0-3.87-3.13-7-7-7z" stroke="currentColor" stroke-width="1.8"/></svg>{{ job.location }}</span>
            <span><svg width="12" height="12" viewBox="0 0 24 24" fill="none"><rect x="2" y="5" width="20" height="14" rx="2" stroke="currentColor" stroke-width="1.8"/></svg>LKR {{ job.salary.toLocaleString() }}</span>
            <span class="badge badge-purple">{{ job.category }}</span>
            <span v-if="job.isRemote" class="badge badge-blue">Remote</span>
          </div>

          <!-- App count -->
          <div class="mc-apps">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none"><path d="M17 21v-2a4 4 0 00-4-4H5a4 4 0 00-4 4v2" stroke="var(--c-text2)" stroke-width="1.8"/><circle cx="9" cy="7" r="4" stroke="var(--c-text2)" stroke-width="1.8"/></svg>
            <strong>{{ appCount(job.id) }}</strong> application{{ appCount(job.id) !== 1 ? 's' : '' }}
          </div>

          <!-- Skills -->
          <div class="mc-skills">
            <span v-for="s in job.requiredSkills.slice(0,3)" :key="s" class="skill-tag">{{ s }}</span>
            <span v-if="job.requiredSkills.length > 3" class="skill-tag">+{{ job.requiredSkills.length - 3 }}</span>
          </div>

          <!-- Actions -->
          <div class="mc-actions">
            <RouterLink :to="`/jobs/${job.id}`" class="btn btn-ghost btn-sm">Preview</RouterLink>
            <RouterLink :to="`/applications?job=${job.id}`" class="btn btn-outline btn-sm">Applications</RouterLink>
            <button class="btn btn-sm" :class="job.isOpen ? 'btn-danger' : 'btn-success'" @click="data.toggleJobStatus(job.id)">
              {{ job.isOpen ? 'Close' : 'Reopen' }}
            </button>
            <button class="btn btn-danger btn-sm" @click="confirmDelete(job.id)">Delete</button>
          </div>
        </div>
      </div>
    </div>

    <!-- Confirm delete modal -->
    <Transition name="fade">
      <div v-if="deleteId !== null" class="modal-overlay" @click.self="deleteId = null">
        <div class="modal-box card">
          <h3>Delete this job?</h3>
          <p>This will permanently remove the listing and cannot be undone.</p>
          <div class="modal-actions">
            <button class="btn btn-ghost" @click="deleteId = null">Cancel</button>
            <button class="btn btn-danger" @click="doDelete">Delete</button>
          </div>
        </div>
      </div>
    </Transition>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'

const auth = useAuthStore()
const data = useDataStore()

const myJobs = computed(() =>
  data.jobs.filter(j => j.company === auth.user?.name || j.company === auth.user?.username),
)

function appCount(jobId: number) {
  return data.getApplicationsByJob(jobId).length
}

const deleteId = ref<number | null>(null)
function confirmDelete(id: number) { deleteId.value = id }
function doDelete() {
  if (deleteId.value !== null) {
    data.removeJob(deleteId.value)
    deleteId.value = null
  }
}
</script>

<style scoped>
.header-row { display: flex; align-items: flex-start; justify-content: space-between; gap: 16px; margin-bottom: 32px; flex-wrap: wrap; }

.manage-card { padding: 20px; display: flex; flex-direction: column; gap: 14px; }
.mc-header { display: flex; justify-content: space-between; align-items: flex-start; gap: 12px; }
.mc-title { font-size: .95rem; font-weight: 600; margin-bottom: 2px; }
.mc-company { font-size: .8rem; color: var(--c-text2); }
.mc-status-badge {
  padding: 4px 10px;
  border-radius: 100px;
  font-size: .72rem;
  font-weight: 600;
  letter-spacing: .04em;
  text-transform: uppercase;
  flex-shrink: 0;
}
.mc-status-badge.open { background: rgba(62,207,142,.12); color: var(--c-green); }
.mc-status-badge.closed { background: rgba(247,95,95,.12); color: var(--c-red); }

.mc-meta { display: flex; flex-wrap: wrap; gap: 10px; font-size: .78rem; color: var(--c-text3); }
.mc-meta span { display: flex; align-items: center; gap: 4px; }

.mc-apps { display: flex; align-items: center; gap: 6px; font-size: .82rem; color: var(--c-text2); }
.mc-skills { display: flex; flex-wrap: wrap; gap: 6px; }
.mc-actions { display: flex; gap: 6px; flex-wrap: wrap; margin-top: auto; }

/* Modal */
.modal-overlay {
  position: fixed; inset: 0;
  background: rgba(0,0,0,.6);
  backdrop-filter: blur(4px);
  display: grid; place-items: center;
  z-index: 300;
  padding: 24px;
}
.modal-box { padding: 32px; max-width: 420px; width: 100%; }
.modal-box h3 { font-size: 1.1rem; margin-bottom: 8px; }
.modal-box p { color: var(--c-text2); font-size: .88rem; }
.modal-actions { display: flex; justify-content: flex-end; gap: 10px; margin-top: 24px; }
</style>